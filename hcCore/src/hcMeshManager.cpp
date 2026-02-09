#include "hc/hcMeshManager.h"
#include "hc/hcModel.h"
#include "hc/hcAssetManager.h"
#include "hc/hcIMeshFactory.h"
#include "hc/hcIMaterialManager.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcIMaterial.h"

namespace hc
{
  MeshManager::MeshManager(
    AssetManager& assetManager,
    IMeshFactory& meshFactory,
    IMaterialManager& materialManager
  ) :
    m_assetManager(assetManager),
    m_meshFactory(meshFactory),
    m_materialManager(materialManager)
  {
  }

  MeshManager::~MeshManager()
  {
  }

  SharedPtr<IMesh> MeshManager::createMeshFromPath(const Path& path)
  {
    SharedPtr<Model> model = m_assetManager.load<Model>(path);
    if (!model)
      return SharedPtr<IMesh>();

    return createMeshFromModel(model);
  }

  SharedPtr<IMesh> MeshManager::createMeshFromModel(
    const SharedPtr<Model>& model
  )
  {
    if (!model)
      return SharedPtr<IMesh>();

    if (hasCachedResource(model->getId()))
      return getCachedResource(model->getId());

    Vector<SharedPtr<IMaterial>> materials = createMaterialsFromModel(model);
    SharedPtr<IMesh> mesh = m_meshFactory.createMesh(
      model,
      materials
    );

    m_meshes.push_back(mesh);
    cacheResource(model->getId(), mesh);

    return mesh;
  }

  const Vector<SharedPtr<IMesh>>& MeshManager::getMeshes() const
  {
    return m_meshes;
  }

  void MeshManager::clear()
  {
    m_meshes.clear();
    clearCache();
  }

  Vector<SharedPtr<IMaterial>> MeshManager::createMaterialsFromModel(
    const SharedPtr<Model>& model
  )
  {
    Vector<SharedPtr<IMaterial>> materials;
    const Vector<SharedPtr<MaterialDescriptor>>& materialDescs = model->getMaterials();
    for (const SharedPtr<MaterialDescriptor>& materialDesc : materialDescs)
    {
      materials.push_back(
        m_materialManager.createMaterialFromDescriptor(materialDesc)
      );
    }

    return materials;
  }
}

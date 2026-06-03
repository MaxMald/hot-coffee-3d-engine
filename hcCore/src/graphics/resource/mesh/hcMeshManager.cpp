#include "hc/graphics/resource/mesh/hcMeshManager.h"
#include "hc/assets/model/hcModel.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"
#include "hc/graphics/resource/mesh/hcIMeshFactory.h"
#include "hc/graphics/resource/material/hcIMaterialManager.h"
#include "hc/graphics/resource/material/hcIMaterial.h"
#include "hc/graphics/resource/mesh/hcIMesh.h"

namespace hc
{
  MeshManager::MeshManager(
    IAssetManager& assetManager,
    UniquePtr<IMeshFactory> meshFactory,
    IMaterialManager& materialManager
  ) :
    m_assetManager(assetManager),
    m_meshFactory(std::move(meshFactory)),
    m_materialManager(materialManager)
  {
  }

  SharedPtr<IMesh> MeshManager::createMesh()
  {
    return m_meshFactory->createMesh();
  }

  SharedPtr<IMesh> MeshManager::createMeshFromPath(const Path& path)
  {
    SharedPtr<Model> model = m_assetManager
      .getModelAssetManager()
      .load(path);

    if (!model)
    {
      LogService::Error(
        String::Format(
          "MeshManager::createMeshFromPath: Failed to load model from path '%s'.",
          path.string().c_str()
        )
      );
      return nullptr;
    }

    return createMeshFromModel(model);
  }

  SharedPtr<IMesh> MeshManager::createMeshFromModel(const SharedPtr<Model>& model)
  {
    if (!model)
    {
      LogService::Error(
        "MeshManager::createMeshFromModel: Invalid model provided."
      );
      return nullptr;
    }

    if (hasCachedResource(model->getId()))
      return getCachedResource(model->getId());

    SharedPtr<IMesh> mesh = m_meshFactory->createMesh();
    if (!mesh)
    {
      LogService::Error(
        String::Format(
          "MeshManager::createMeshFromModel: Failed to create mesh from model with Id '%s'.",
          model->getId().toString().c_str()
        )
      );
      return nullptr;
    }

    Vector<SharedPtr<IMaterial>> materials = createMaterialsFromModel(model);

    mesh->initialize(*model, materials);
    if (!mesh->isValid())
    {
      LogService::Error(
        String::Format(
          "MeshManager::createMeshFromModel: Failed to initialize mesh from model with Id '%s'.",
          model->getId().toString().c_str()
        )
      );
      return nullptr;
    }

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
    const Vector<SharedPtr<AMaterialDescriptor>>& materialDescs = model->getMaterials();
    for (const SharedPtr<AMaterialDescriptor>& materialDesc : materialDescs)
    {
      SharedPtr<IMaterial> createdMaterial = m_materialManager
        .createMaterialFromDescriptor(materialDesc);

      if (!createdMaterial)
      {
        throw RuntimeErrorException(
          String::Format(
            "MeshManager::createMaterialsFromModel: Failed to create material from descriptor with Id '%s'.",
            materialDesc->getId().toString().c_str()
          )
        );

        continue;
      }

      materials.push_back(createdMaterial);
    }

    return materials;
  }
}

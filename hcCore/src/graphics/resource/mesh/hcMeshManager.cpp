#include "hc/graphics/resource/mesh/hcMeshManager.h"
#include "hc/assets/hcModel.h"
#include "hc/assets/hcAssetManager.h"
#include "hc/graphics/resource/mesh/hcIMeshFactory.h"
#include "hc/graphics/resource/material/hcIMaterialManager.h"
#include "hc/assets/hcMaterialDescriptor.h"
#include "hc/graphics/resource/material/hcIMaterial.h"

namespace hc
{
  MeshManager::MeshManager(
    AssetManager& assetManager,
    UniquePtr<IMeshFactory> meshFactory,
    IMaterialManager& materialManager
  ) :
    m_assetManager(assetManager),
    m_meshFactory(std::move(meshFactory)),
    m_materialManager(materialManager)
  {
  }

  SharedPtr<IMesh> MeshManager::createMeshFromPath(const Path& path)
  {
    SharedPtr<Model> model = m_assetManager.load<Model>(path);
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

  SharedPtr<IMesh> MeshManager::createMeshFromModel(
    const SharedPtr<Model>& model
  )
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

    Vector<SharedPtr<IMaterial>> materials = createMaterialsFromModel(model);
    SharedPtr<IMesh> mesh = m_meshFactory->createMesh(
      model,
      materials
    );

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

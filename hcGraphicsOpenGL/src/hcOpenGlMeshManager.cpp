#include "hc/hcOpenGlMeshManager.h"
#include "hc/hcOpenGlMesh.h"

namespace hc
{
  OpenGlMeshManager::OpenGlMeshManager() :
    m_assetManager(nullptr),
    m_materialManager(nullptr)
  {
  }

  OpenGlMeshManager::~OpenGlMeshManager()
  {
  }

  SharedPtr<IMesh> OpenGlMeshManager::createMeshFromPath(const Path& path)
  {
    SharedPtr<Model> model = m_assetManager->load<Model>(path);
    if (!model)
      return SharedPtr<IMesh>();

    return createMeshFromModel(model);
  }

  SharedPtr<IMesh> OpenGlMeshManager::createMeshFromModel(
    const SharedPtr<Model>& model
  )
  {
    Id modelId = model->getId();
    auto it = m_cachedMeshes.find(modelId);
    if (it != m_cachedMeshes.end())
      return it->second;

    SharedPtr<OpenGlMesh> mesh = MakeShared<OpenGlMesh>(
      model,
      m_materialManager
    );

    m_cachedMeshes[modelId] = mesh;
    return mesh;
  }

  void OpenGlMeshManager::initialize(
    SharedPtr<AssetManager> assetManager,
    OpenGlMaterialManager* materialManager
  )
  {
    m_assetManager = assetManager;
    m_materialManager = materialManager;
  }
}

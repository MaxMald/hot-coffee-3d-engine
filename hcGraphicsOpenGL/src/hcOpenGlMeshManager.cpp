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
    if (!model)
      return SharedPtr<IMesh>();

    if (hasCachedMeshForModelId(model->getId()))
      return m_cachedMeshesByModelId[model->getId()];

    SharedPtr<OpenGlMesh> mesh = MakeShared<OpenGlMesh>(
      model,
      m_materialManager
    );

    addMesh(mesh);
    return mesh;
  }

  const Vector<SharedPtr<IMesh>>& OpenGlMeshManager::getMeshes() const
  {
    return m_meshes;
  }

  void OpenGlMeshManager::clear()
  {
    m_meshes.clear();
    m_cachedMeshesByModelId.clear();
  }

  void OpenGlMeshManager::initialize(
    SharedPtr<AssetManager> assetManager,
    OpenGlMaterialManager* materialManager
  )
  {
    m_assetManager = assetManager;
    m_materialManager = materialManager;
  }

  bool OpenGlMeshManager::hasCachedMeshForModelId(const Id& modelId) const
  {
    return m_cachedMeshesByModelId.find(modelId) != 
      m_cachedMeshesByModelId.end();
  }

  void OpenGlMeshManager::addMesh(const SharedPtr<OpenGlMesh>& mesh)
  {
    m_meshes.push_back(mesh);
    m_cachedMeshesByModelId[mesh->getModel()->getId()] = mesh;
  }
}

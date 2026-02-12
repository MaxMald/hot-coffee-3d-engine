#include "hc/hcScene.h"
#include "hc/hcRenderContext.h"
#include "hc/hcCamera.h"
#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  Scene::Scene(IGameObjectFactory& gameObjectFactory) :
    m_gameObjectFactory(gameObjectFactory)
  {
  }

  Scene::~Scene()
  {
  }

  UniquePtr<GameObject> Scene::createGameObject(const String& name) const
  {
    return m_gameObjectFactory.create(name);
  }

  GameObject* Scene::createRootGameObject(const String& name)
  {
    UniquePtr<GameObject> gameObject = createGameObject(name);
    if (!gameObject)
      throw RuntimeErrorException(
        String::Format("Failed to create GameObject with name '%s'", name.c_str())
      );

    GameObject* gameObjectPtr = gameObject.get();
    m_sceneGraph.addRoot(std::move(gameObject));
    return gameObjectPtr;
  }

  void Scene::draw()
  {
    RenderContext renderContext;

    Camera* activeCamera = m_cameraManager.getActiveCamera();
    if (activeCamera)
    {
      renderContext.cameraMatrices.viewMatrix = activeCamera->getViewMatrix();
      renderContext.cameraMatrices.projectionMatrix = activeCamera->getProjectionMatrix();
    }
    else
    {
      Camera& defaultCamera = m_cameraManager.getDefaultCamera();
      renderContext.cameraMatrices.viewMatrix = defaultCamera.getViewMatrix();
      renderContext.cameraMatrices.projectionMatrix = defaultCamera.getProjectionMatrix();
    }

    renderContext.transform = Matrix4::Identity();

    m_sceneGraph.draw(renderContext);
  }

  void Scene::update(const Time& deltaTime)
  {
    m_sceneGraph.update(deltaTime);
  }

  SceneGraph& Scene::getSceneGraph()
  {
    return m_sceneGraph;
  }

  const SceneGraph& Scene::getSceneGraph() const
  {
    return m_sceneGraph;
  }

  LightManager& Scene::getLightManager()
  {
    return m_lightManager;
  }

  const LightManager& Scene::getLightManager() const
  {
    return m_lightManager;
  }

  CameraManager& Scene::getCameraManager()
  {
    return m_cameraManager;
  }

  const CameraManager& Scene::getCameraManager() const
  {
    return m_cameraManager;
  }

  void Scene::onActivate()
  {
    // TODO
  }

  void Scene::onDeactivate()
  {
    // TODO
  }

  void Scene::onDestroy()
  {
    m_lightManager.clear();
  }
}

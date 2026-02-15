#include "hc/hcScene.h"
#include "hc/hcRenderContext.h"
#include "hc/hcCamera.h"
#include "hc/hcIGameObjectFactory.h"

namespace hc
{
  Scene::Scene() :
    m_gameObjectFactory(nullptr)
  {
  }

  Scene::~Scene()
  {
  }

  UniquePtr<GameObject> Scene::createGameObject(const String& name)
  {
    if (!m_gameObjectFactory)
    {
      throw RuntimeErrorException(
        "GameObjectFactory is not initialized. Cannot create GameObject."
      );
    }

    return m_gameObjectFactory->create(name);
  }

  GameObject* Scene::createRootGameObject(const String& name)
  {
    UniquePtr<GameObject> root = createGameObject(name);
    if (!root)
    {
      throw RuntimeErrorException(
        String::Format("Failed to create root GameObject with name '%s'", name.c_str())
      );
    }

    GameObject* rootPtr = root.get();
    m_sceneGraph.addRoot(std::move(root));
    return rootPtr;
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

  void Scene::onInitialized()
  {
    // This method can be overridden by derived classes to perform actions after
    // the scene is initialized.
  }

  void Scene::onActivate()
  {
    // This method can be overridden by derived classes to perform actions when
    // the scene is activated.
  }

  void Scene::onDeactivate()
  {
    // This method can be overridden by derived classes to perform actions when
    // the scene is deactivated.
  }

  void Scene::onLoad()
  {
    // This method can be overridden by derived classes to load resources to the
    // scene when it is activated.
  }

  void Scene::onPrepare()
  {
    // This method can be overridden by derived classes to perform any necessary
    // preparation after loading resources and before the first update/draw.
  }

  void Scene::onBeforeDraw(const RenderContext&)
  {
    // This method can be overridden by derived classes to perform actions before
    // the scene graph is drawn.
  }

  void Scene::onAfterDraw(const RenderContext&)
  {
    // This method can be overridden by derived classes to perform actions after
    // the scene graph is drawn.
  }

  void Scene::onBeforeUpdate(const Time&)
  {
    // This method can be overridden by derived classes to perform actions before
    // the scene graph is updated.
  }

  void Scene::onAfterUpdate(const Time&)
  {
    // This method can be overridden by derived classes to perform actions after
    // the scene graph is updated.
  }

  void Scene::onDestroy()
  {
    // This method can be overridden by derived classes to perform cleanup when
    // the scene is destroyed.
  }

  void Scene::initialize(IGameObjectFactory* gameObjectFactory)
  {
    if (!gameObjectFactory)
      throw InvalidArgumentException("GameObjectFactory pointer cannot be null");

    m_gameObjectFactory = gameObjectFactory;
    onInitialized();
  }

  void Scene::activate()
  {
    onActivate();

    // Note: onLoad() is called every time the scene is activated.
    // Since deactivate() calls clear(), scenes must (re)load or recreate
    // their resources in onLoad() whenever they become active again.
    onLoad();
    onPrepare();
  }

  void Scene::deactivate()
  {
    onDeactivate();
    clear();
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

    onBeforeDraw(renderContext);
    m_sceneGraph.draw(renderContext);
    onAfterDraw(renderContext);
  }

  void Scene::update(const Time& elapsedTime)
  {
    onBeforeUpdate(elapsedTime);
    m_sceneGraph.update(elapsedTime);
    onAfterUpdate(elapsedTime);
  }

  void Scene::clear()
  {
    m_sceneGraph.clear();
    m_lightManager.clear();
    m_cameraManager.clear();
  }

  void Scene::destroy()
  {
    onDestroy();
    clear();
  }
}

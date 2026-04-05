#include "hc/scene/hcScene.h"
#include "hc/graphics/hcRenderContext.h"
#include "hc/scene/camera/hcCamera.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"

namespace hc
{
  Scene::Scene() :
    m_sceneGraph(),
    m_cameraManager(),
    m_lightManager(),
    m_gameObjectFactory(nullptr)
  {
  }

  Scene::~Scene()
  {
  }

  void Scene::serialize(BinaryWriter& writer) const
  {
    m_lightManager.serialize(writer);
    m_cameraManager.serialize(writer);
    m_sceneGraph.serialize(writer);
    onSerialize(writer);
  }

  void Scene::deserialize(BinaryReader& reader)
  {
    clear();
    m_lightManager.deserialize(reader);
    m_cameraManager.deserialize(reader);
    m_sceneGraph.deserialize(reader);
    onDeserialize(reader);
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

  void Scene::draw()
  {
    Camera* activeCamera = m_cameraManager.getActiveCamera();
    if (activeCamera)
      draw(activeCamera);
    else
      draw(&(m_cameraManager.getDefaultCamera()));
  }

  void Scene::draw(Camera* camera)
  {
    if (!camera)
    {
      throw RuntimeErrorException(
        "Scene::draw: Null camera provided. Drawing cannot proceed."
      );
    }

    RenderContext renderContext;
    renderContext.cameraMatrices.viewMatrix = camera->getViewMatrix();
    renderContext.cameraMatrices.projectionMatrix = camera->getProjectionMatrix();
    renderContext.cameraPosition = camera->getPosition();
    renderContext.transform = Matrix4::Identity();
    renderContext.modelPosition = Vector3f(0.0f, 0.0f, 0.0f);

    onBeforeDraw(renderContext);
    m_sceneGraph.draw(renderContext);
    onAfterDraw(renderContext);
  }

  void Scene::clear()
  {
    m_sceneGraph.clear();
    m_lightManager.clear();
    m_cameraManager.clear();
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

  void Scene::onSerialize(BinaryWriter&) const
  {
    // This method can be overridden by derived classes to write custom data during
    // serialization. The base implementation serializes the scene graph and
    // default camera.
  }

  void Scene::onDeserialize(BinaryReader&)
  {
    // This method can be overridden by derived classes to read custom data during
    // deserialization. The base implementation deserializes the scene graph and
    // default camera.
  }

  void Scene::initialize(IGameObjectFactory* gameObjectFactory)
  {
    if (!gameObjectFactory)
      throw InvalidArgumentException("GameObjectFactory pointer cannot be null");

    m_gameObjectFactory = gameObjectFactory;
    m_sceneGraph.initialize(gameObjectFactory);
    onInitialized();
  }

  void Scene::activate()
  {
    onActivate();
    onLoad();
    onPrepare();
  }

  void Scene::deactivate()
  {
    onDeactivate();
  }

  void Scene::update(const Time& elapsedTime)
  {
    onBeforeUpdate(elapsedTime);
    m_sceneGraph.update(elapsedTime);
    onAfterUpdate(elapsedTime);
  }

  void Scene::destroy()
  {
    onDestroy();
    clear();
  }
}

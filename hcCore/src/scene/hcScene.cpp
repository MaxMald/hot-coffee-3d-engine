#include "hc/scene/hcScene.h"
#include "hc/graphics/hcRenderContext.h"
#include "hc/graphics/hcIGraphicsManager.h"
#include "hc/graphics/lightFrameData/hcSceneGraphLightFrameDataGatherer.h"
#include "hc/scene/camera/hcCamera.h"
#include "hc/scene/gameObject/hcIGameObjectFactory.h"

namespace hc
{
  Scene::Scene() :
    m_sceneGraph(),
    m_cameraManager(),
    m_lightFrameData(),
    m_gameObjectFactory(nullptr)
  {
  }

  Scene::~Scene()
  {
  }

  void Scene::serialize(BinaryWriter& writer) const
  {
    m_cameraManager.serialize(writer);
    m_sceneGraph.serialize(writer);
    onSerialize(writer);
  }

  void Scene::deserialize(BinaryReader& reader)
  {
    clear();
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

  CameraManager& Scene::getCameraManager()
  {
    return m_cameraManager;
  }

  const CameraManager& Scene::getCameraManager() const
  {
    return m_cameraManager;
  }

  void Scene::draw(IGraphicsManager& graphicsManager)
  {
    Camera* activeCamera = m_cameraManager.getActiveCamera();
    if (activeCamera)
      draw(graphicsManager, activeCamera);
    else
      draw(graphicsManager, &(m_cameraManager.getDefaultCamera()));
  }

  void Scene::draw(IGraphicsManager& graphicsManager, Camera* camera)
  {
    if (!camera)
    {
      throw RuntimeErrorException(
        "Scene::draw: Null camera provided. Drawing cannot proceed."
      );
    }

    camera->update();
    graphicsManager.uploadCameraRenderData(CameraRenderData::Create(*camera));

    // Gather light frame data and upload it to the graphics manager

    m_lightFrameData.numDirectionalLights = 0;
    m_lightFrameData.numOmniLights = 0;
    m_lightFrameData.numSpotLights = 0;
    SceneGraphLightFrameDataGatherer::Gather(m_sceneGraph, m_lightFrameData);

    graphicsManager.uploadLightFrameData(m_lightFrameData);

    // Draw the scene graph with the provided render context
    RenderContext renderContext = RenderContext::Create(*camera, Matrix4::Identity());

    onBeforeDraw(renderContext);
    m_sceneGraph.draw(renderContext);
    onAfterDraw(renderContext);
  }

  void Scene::clear()
  {
    m_sceneGraph.clear();
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

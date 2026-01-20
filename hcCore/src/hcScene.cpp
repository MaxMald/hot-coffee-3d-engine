#include "hc/hcScene.h"

namespace hc
{
  Scene::Scene()
  {
  }

  Scene::~Scene()
  {
  }

  void Scene::draw(IGraphicsManager& graphicsManager)
  {
    m_sceneGraph.draw(graphicsManager);
  }

  void Scene::update(float deltaTime)
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

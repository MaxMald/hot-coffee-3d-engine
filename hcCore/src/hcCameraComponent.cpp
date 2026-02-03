#include "hc/hcCameraComponent.h"
#include "hc/hcHotCoffeeEngine.h"
#include "hc/hcSceneManager.h"
#include "hc/hcCameraManager.h";

namespace hc
{
  CameraComponent::CameraComponent() :
    ABaseComponent(componentType::Type::Camera),
    m_camera(nullptr)
  {
    CameraManager& cameraManager = getCameraManager();
    m_camera = cameraManager.createCamera();
  }

  CameraComponent::~CameraComponent()
  {
    CameraManager& cameraManager = getCameraManager();
    cameraManager.destroyCamera(m_camera);
  }

  void CameraComponent::setAsActiveCamera()
  {
    getCameraManager().setActiveCamera(m_camera);
  }

  CameraManager& CameraComponent::getCameraManager()
  {
    SceneManager& sceneManager = HotCoffeeEngine::Instance().getSceneManager();
    Scene* activeScene = sceneManager.getActiveScene();
    if (!activeScene)
      throw RuntimeErrorException(
        "Cannot get CameraManager because there is no active scene."
      );

    return activeScene->getCameraManager();
  }
}

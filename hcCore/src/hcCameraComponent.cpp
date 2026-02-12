#include "hc/hcCameraComponent.h"
#include "hc/hcHotCoffeeEngine.h"
#include "hc/hcSceneManager.h"
#include "hc/hcCameraManager.h"
#include "hc/hcCamera.h"

namespace hc
{
  CameraComponent::CameraComponent(SceneManager& sceneManager) :
    ABaseComponent(componentType::Type::Camera),
    m_sceneManager(sceneManager),
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

  const Vector3f& CameraComponent::getPosition() const 
  {
    if (m_camera)
      return m_camera->getPosition();
    return Vector3f(0.0f, 0.0f, 0.0f);
  }

  const Vector3f& CameraComponent::getDirection() const
  {
    if (m_camera)
      return m_camera->getDirection();
    return Vector3f(0.0f, 0.0f, 0.0f);
  }

  const Vector3f& CameraComponent::getUp() const
  {
    if (m_camera)
      return m_camera->getUp();
    return Vector3f(0.0f, 0.0f, 0.0f);
  }

  void CameraComponent::setAsActiveCamera()
  {
    if (m_camera)
      getCameraManager().setActiveCamera(m_camera);
  }

  void CameraComponent::setProjectionType(projectionType::Type type)
  {
    if (m_camera)
      m_camera->setProjectionType(type);
  }

  Matrix4 CameraComponent::getProjectionMatrix()
  {
    if (m_camera)
      return m_camera->getProjectionMatrix();
    return Matrix4::Identity();
  }

  Matrix4 CameraComponent::getViewMatrix()
  {
    if (m_camera)
      return m_camera->getViewMatrix();
    return Matrix4::Identity();
  }

  ICameraProjection* CameraComponent::getCameraProjection()
  {
    if (m_camera)
      return m_camera->getCameraProjection();
    return nullptr;
  }

  CameraManager& CameraComponent::getCameraManager()
  {
    Scene* activeScene = m_sceneManager.getActiveScene();
    if (!activeScene)
      throw RuntimeErrorException(
        "Cannot get CameraManager because there is no active scene."
      );

    return activeScene->getCameraManager();
  }
}

#include "hc/scene/gameObject/components/hcCameraComponent.h"
#include "hc/scene/hcSceneManager.h"
#include "hc/scene/camera/hcCameraManager.h"
#include "hc/scene/camera/hcCamera.h"

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
    assertCameraExists();
    return m_camera->getPosition();
  }

  const Vector3f& CameraComponent::getDirection() const
  {
    assertCameraExists();
    return m_camera->getDirection();
  }

  const Vector3f& CameraComponent::getUp() const
  {
    assertCameraExists();
    return m_camera->getUp();
  }

  void CameraComponent::setAsActiveCamera()
  {
    assertCameraExists();
    getCameraManager().setActiveCamera(m_camera);
  }

  void CameraComponent::setProjectionType(projectionType::Type type)
  {
    assertCameraExists();
    m_camera->setProjectionType(type);
  }

  Matrix4 CameraComponent::getProjectionMatrix()
  {
    assertCameraExists();
    return m_camera->getProjectionMatrix();
  }

  Matrix4 CameraComponent::getViewMatrix()
  {
    assertCameraExists();
    return m_camera->getViewMatrix();
  }

  ICameraProjection* CameraComponent::getCameraProjection()
  {
    assertCameraExists();
    return m_camera->getCameraProjection();
  }

  void CameraComponent::onSerialize(BinaryWriter& writer) const
  {
    assertCameraExists();
    m_camera->serialize(writer);
  }

  void CameraComponent::onDeserialize(BinaryReader& reader)
  {
    assertCameraExists();
    m_camera->deserialize(reader);
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

  void CameraComponent::assertCameraExists() const
  {
    if (!m_camera)
      throw RuntimeErrorException(
        "CameraComponent does not have a valid Camera instance."
      );
  }
}

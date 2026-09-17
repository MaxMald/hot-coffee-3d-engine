#include "hc/editor/views/windows/sceneViewport/hcSceneViewportCamera.h"

namespace hc::editor
{
  static constexpr float MINIMUM_DISTANCE_TO_TARGET = 0.01f;
  static constexpr float EPSILON = 0.0001f;

  SceneViewportCamera::SceneViewportCamera() :
    m_camera(),
    m_target(0.0f, 0.0f, 0.0f),
    m_cameraOrbitSensitivity(0.010f),
    m_cameraMovementScale(0.005f),
    m_cameraScrollSensitivity(0.1f),
    m_rollSensitivity(0.01f)
  {
    setTarget(Vector3f(0.0f, 0.0f, MINIMUM_DISTANCE_TO_TARGET));
  }

  Camera& SceneViewportCamera::getCamera()
  {
    return m_camera;
  }

  const Camera& SceneViewportCamera::getCamera() const
  {
    return m_camera;
  }

  Vector3f SceneViewportCamera::getTarget() const
  {
    return m_target;
  }

  void SceneViewportCamera::setTarget(const Vector3f& target)
  {
    float expectedDistance = (target - m_camera.getPosition()).length();
    if (Math::IsNearlyEqual(expectedDistance, 0.0f, EPSILON))
    {
      m_camera.setPosition(target + Vector3f(0.0f, 0.0f, MINIMUM_DISTANCE_TO_TARGET));
    }
    else if (expectedDistance < MINIMUM_DISTANCE_TO_TARGET)
    {
      Vector3f direction = (target - m_camera.getPosition()).normalized();
      m_camera.setPosition(target - direction * MINIMUM_DISTANCE_TO_TARGET);
    }

    m_target = target;
    m_camera.lookAt(m_target, Vector3f(0.0f, 1.0f, 0.0f));
  }

  float SceneViewportCamera::getDistanceToTarget() const
  {
    return (m_target - m_camera.getPosition()).length();
  }

  void SceneViewportCamera::setDistanceToTarget(float distance)
  {
    distance = Math::Max(distance, MINIMUM_DISTANCE_TO_TARGET);
    Vector3f direction = (m_camera.getPosition() - m_target).normalized();
    m_camera.setPosition(m_target + direction * distance);
  }

  float SceneViewportCamera::getCameraOrbitSensitivity() const
  {
    return m_cameraOrbitSensitivity;
  }

  void SceneViewportCamera::setCameraOrbitSensitivity(float sensitivity)
  {
    m_cameraOrbitSensitivity = Math::Max(sensitivity, 0.0f);
  }

  float SceneViewportCamera::getCameraMovementScale() const
  {
    return m_cameraMovementScale;
  }

  void SceneViewportCamera::setCameraMovementScale(float scale)
  {
    m_cameraMovementScale = Math::Max(scale, 0.0f);
  }

  float SceneViewportCamera::getCameraScrollSensitivity() const
  {
    return m_cameraScrollSensitivity;
  }

  void SceneViewportCamera::setCameraScrollSensitivity(float sensitivity)
  {
    m_cameraScrollSensitivity = Math::Max(sensitivity, 0.0f);
  }

  float SceneViewportCamera::getRollSensitivity() const
  {
    return m_rollSensitivity;
  }

  void SceneViewportCamera::setRollSensitivity(float sensitivity)
  {
    m_rollSensitivity = Math::Max(sensitivity, 0.0f);
  }

  Vector3f SceneViewportCamera::getCameraPosition() const
  {
    return m_camera.getPosition();
  }

  void SceneViewportCamera::setCameraPosition(const Vector3f& position)
  {
    float expectedDistance = (m_target - position).length();
    if (Math::IsNearlyEqual(expectedDistance, 0.0f, EPSILON))
    {
      m_camera.setPosition(m_target + Vector3f(0.0f, 0.0f, MINIMUM_DISTANCE_TO_TARGET));
    }
    else if (expectedDistance < MINIMUM_DISTANCE_TO_TARGET)
    {
      Vector3f direction = (position - m_target).normalized();
      m_camera.setPosition(m_target + direction * MINIMUM_DISTANCE_TO_TARGET);
    }
    else
    {
      m_camera.setPosition(position);
    }

    m_camera.lookAt(m_target, Vector3f(0.0f, 1.0f, 0.0f));
  }

  projectionType::Type SceneViewportCamera::getCameraProjectionType() const
  {
    return m_camera.getProjection();
  }

  void SceneViewportCamera::setCameraProjectionType(projectionType::Type type)
  {
    m_camera.setProjectionType(type);
  }

  Angle SceneViewportCamera::getCameraFovY() const
  {
    if (m_camera.getProjection() != projectionType::Type::Perspective)
      return Angle::FromDegrees(0.0f);

    const ICameraProjection* projection = m_camera.getCameraProjection();
    const PerspectiveCameraProjection* perspectiveProjection
      = reinterpret_cast<const PerspectiveCameraProjection*>(projection);
    return perspectiveProjection->getFovY();
  }

  void SceneViewportCamera::setCameraFovY(const Angle& fov)
  {
    if (m_camera.getProjection() != projectionType::Type::Perspective)
      return;

    ICameraProjection* projection = m_camera.getCameraProjection();
    PerspectiveCameraProjection* perspectiveProjection
      = reinterpret_cast<PerspectiveCameraProjection*>(projection);
    perspectiveProjection->setFovY(fov);
  }

  void SceneViewportCamera::orbit(const Vector2f & mouseDelta)
  {
    float yaw = -mouseDelta.x * m_cameraOrbitSensitivity;
    float pitch = -mouseDelta.y * m_cameraOrbitSensitivity;

    Matrix4 yawRotation = Matrix4::RotationAxis(m_camera.getUp(), yaw);
    Vector3f rightAfterYaw = (yawRotation * Vector4f(m_camera.getRight(), 0.0f)).xyz();
    Matrix4 pitchRotation = Matrix4::RotationAxis(rightAfterYaw, pitch);
    Matrix4 combinedRotation = yawRotation * pitchRotation;

    Vector3f targetToCamera = m_camera.getPosition() - m_target;
    Vector3f rotatedVector = (combinedRotation * Vector4f(targetToCamera, 0.0f)).xyz();

    float cosAngle = rotatedVector.normalized().dot(Vector3f(0.0f, 1.0f, 0.0f));
    if (Math::IsNearlyEqual(cosAngle, 1.0f, EPSILON) || Math::IsNearlyEqual(cosAngle, -1.0f, EPSILON))
      return;

    m_camera.setPosition(m_target + rotatedVector);
    m_camera.lookAt(m_target, Vector3f(0.0f, 1.0f, 0.0f));
  }

  void SceneViewportCamera::dolly(float delta)
  {
    m_target += m_camera.getDirection() * delta * m_cameraMovementScale;
    m_camera.dolly(delta * m_cameraMovementScale);
  }

  void SceneViewportCamera::truck(float delta)
  {
    m_target += m_camera.getRight() * delta * m_cameraMovementScale;
    m_camera.truck(delta * m_cameraMovementScale);
  }

  void SceneViewportCamera::pedestal(float delta)
  {
    m_target += m_camera.getUp() * delta * m_cameraMovementScale;
    m_camera.pedestal(delta * m_cameraMovementScale);
  }

  void SceneViewportCamera::zoom(float scrollDelta)
  {
    float currentDistance = (m_target - m_camera.getPosition()).length();
    float deltaZ = scrollDelta * m_cameraScrollSensitivity;
    float maxAllowedDelta = currentDistance - MINIMUM_DISTANCE_TO_TARGET;
    if (deltaZ > maxAllowedDelta)
      deltaZ = maxAllowedDelta;

    m_camera.dolly(deltaZ);
  }

  void SceneViewportCamera::roll(const Angle& rollAmount)
  {
    m_camera.roll(rollAmount * m_rollSensitivity);
  }
}

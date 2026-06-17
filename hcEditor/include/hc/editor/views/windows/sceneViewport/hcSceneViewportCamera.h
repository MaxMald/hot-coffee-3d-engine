#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class SceneViewportCamera
  {
  public:
    SceneViewportCamera();
    ~SceneViewportCamera() = default;

    Camera& getCamera();
    const Camera& getCamera() const;
    Vector3f getTarget() const;
    void setTarget(const Vector3f& target);
    float getDistanceToTarget() const;
    void setDistanceToTarget(float distance);
    float getCameraOrbitSensitivity() const;
    void setCameraOrbitSensitivity(float sensitivity);
    float getCameraMovementScale() const;
    void setCameraMovementScale(float scale);
    float getCameraScrollSensitivity() const;
    void setCameraScrollSensitivity(float sensitivity);
    float getRollSensitivity() const;
    void setRollSensitivity(float sensitivity);
    Vector3f getCameraPosition() const;
    void setCameraPosition(const Vector3f& position);
    void orbit(const Vector2f& mouseDelta);
    void dolly(float delta);
    void truck(float delta);
    void pedestal(float delta);
    void zoom(float scrollDelta);
    void roll(const Angle& rollAmount);

  private:
    Camera m_camera;
    Vector3f m_target;
    float m_cameraOrbitSensitivity;
    float m_cameraMovementScale;
    float m_cameraScrollSensitivity;
    float m_rollSensitivity;
  };
}

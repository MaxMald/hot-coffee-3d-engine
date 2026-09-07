#pragma once

#include "hc/scene/gameObject/components/hcABaseComponent.h"
#include "hc/scene/camera/hcProjectionType.h"

namespace hc
{
  class Camera;
  class CameraManager;
  class SceneManager;
  class ICameraProjection;

  /**
   * @brief Component that encapsulates camera functionality for entities.
   */
  class HC_CORE_EXPORT CameraComponent : public ABaseComponent
  {
  public:
    CameraComponent(SceneManager& sceneManager);

    ~CameraComponent() override;

    void serialize(io::BinaryWriter& writer) const override;

    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Gets the camera's position in world space.
     *
     * @return Reference to the position vector.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Gets the camera's direction vector.
     *
     * @return Reference to the direction vector.
     */
    const Vector3f& getDirection() const;

    /**
     * @brief Gets the camera's up vector.
     *
     * @return Reference to the up vector.
     */
    const Vector3f& getUp() const;

    /**
     * @brief Sets the camera's projection type.
     *
     * @param projectionType The desired projection type.
     */
    void setProjectionType(projectionType::Type projectionType);

    /**
     * @brief Gets the camera's projection matrix.
     *
     * @return The projection matrix.
     */
    Matrix4 getProjectionMatrix();

    /**
     * @brief Gets the camera's view matrix.
     *
     * @return The view matrix.
     */
    Matrix4 getViewMatrix();

    /**
     * @brief Gets the active camera projection object.
     *
     * @return Pointer to the camera projection.
     */
    ICameraProjection* getCameraProjection();

    /**
     * @brief Sets this camera as the active camera in the scene.
     */
    void setAsActiveCamera();

  private:
    SceneManager& m_sceneManager;
    Camera* m_camera;

    /**
     * @brief Gets the camera manager associated with this component.
     *
     * @return Reference to the camera manager.
     */
    CameraManager& getCameraManager();

    /**
     * @brief Asserts that the camera exists and is valid.
     *
     * @throws RuntimeErrorException if the camera does not exist.
     */
    void assertCameraExists() const;
  };
}

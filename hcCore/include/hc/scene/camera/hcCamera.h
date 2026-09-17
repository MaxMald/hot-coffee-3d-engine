#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/camera/hcProjectionType.h"
#include "hc/scene/camera/hcPerspectiveCameraProjection.h"
#include "hc/scene/camera/hcOrthographicCameraProjection.h"

namespace hc
{
  namespace dataBlockStructure
  {
    struct Camera;
  }

  /**
   * @brief Represents a camera in the engine.
   */
  class HC_CORE_EXPORT Camera : public io::ISerializable
  {
  public:
    Camera();
    ~Camera();

    /**
     * @copydoc ISerializable::serialize
     */
    void serialize(io::BinaryWriter& writer) const override;

    /**
     * @copydoc ISerializable::deserialize
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Gets the unique identifier of the camera.
     *
     * @return The camera's UUID.
     */
    const UUID& getUUID() const;

    /**
     * @brief Updates the camera's internal state.
     */
    void update();

    /**
     * @brief Sets the aspect ratio of the camera.
     * 
     * @param aspectRatio The new aspect ratio (width/height).
     */
    void setAspectRatio(float aspectRatio);

    /**
     * @brief Sets the aspect ratio of the camera based on the given viewport dimensions.
     * 
     * @param width The width of the viewport.
     * @param height The height of the viewport.
     */
    void setAspectRatio(UInt32 width, UInt32 height);

    /**
     * @brief Sets the position of the camera.
     *
     * @param position The new position.
     */
    void setPosition(const Vector3f& position);

    /**
     * @brief Gets the position of the camera.
     *
     * @return The current position.
     */
    const Vector3f& getPosition() const;

    /**
     * @brief Sets the direction the camera is looking at.
     *
     * @param direction The new direction vector.
     */
    void setDirection(const Vector3f& direction);

    /**
     * @brief Gets the direction the camera is looking at.
     *
     * @return The current direction vector.
     */
    const Vector3f& getDirection() const;

    /**
     * @brief Rolls the camera around its forward axis by a specified angle.
     *
     * @param rollAmount The angle to roll the camera (in radians or degrees,
     * depending on Angle type).
     */
    void roll(const Angle& rollAmount);

    /**
     * @brief Tilts the camera up or down by a specified angle.
     *
     * @param tiltAmount The angle to tilt the camera (in radians or degrees,
     * depending on Angle type). Positive values tilt it up, while negative
     * values tilt it down.
     */
    void tilt(const Angle& tiltAmount);

    /**
     * @brief Pans the camera left or right by a specified angle.
     *
     * @param panAmount The angle to pan the camera (in radians or degrees,
     * depending on Angle type). Positive values pan it right, while negative
     * values pan it left.
     */
    void pan(const Angle& panAmount);

    /**
     * @brief Dollies the camera forward or backward along its direction vector.
     *
     * @param delta The distance to move the camera. Positive values move it
     * forward, while negative values move it backward.
     */
    void dolly(float delta);

    /**
     * @brief Pedestals the camera up or down along its up vector.
     *
     * @param delta The distance to move the camera. Positive values move it
     * up, while negative values move it down.
     */
    void pedestal(float delta);

    /**
     * @brief Trucks the camera left or right along its right vector.
     *
     * @param delta The distance to move the camera. Positive values move it
     * right, while negative values move it left.
     */
    void truck(float delta);

    /**
     * @brief Gets the up vector of the camera.
     *
     * @return The current up vector.
     */
    const Vector3f& getUp() const;

    /**
     * @brief Gets the right vector of the camera, computed as the cross product
     * of the direction and up vectors.
     *
     * @return The right vector.
     */
    Vector3f getRight() const;

    /**
     * @brief Moves the camera by a specified delta vector.
     *
     * @param delta The vector by which to move the camera.
     */
    void move(const Vector3f& delta);

    /**
     * @brief Orients the camera to look at a specific target point in world space.
     *
     * @param target The point in world space to look at.
     */
    void lookAt(const Vector3f& target);

    /**
     * @brief Orients the camera to look at a specific target point in world space,
     * using a specified up vector to determine the camera's roll.
     *
     * @param target The point in world space to look at.
     * @param up The up vector to use for orienting the camera. This determines
     * the camera's roll around the direction vector. It should not be parallel
     * to the direction vector.
     */
    void lookAt(const Vector3f& target, const Vector3f& up);

    /**
     * @brief Rotates the camera by specified Euler angles (in radians).
     *
     * @param eulerAngles The rotation angles around the X, Y, and Z axes.
     */
    void rotate(const Vector3f& eulerAngles);

    /**
     * @brief Sets the projection type of the camera.
     * 
     * @param type The new projection type.
     */
    void setProjectionType(projectionType::Type type);

    /*
    * @brief Gets the projection type of the camera.
    * 
    * @returns The current projection type.
    */
    projectionType::Type getProjection() const;

    /**
     * @brief Computes and returns the view matrix of the camera.
     *
     * @return The view matrix.
     */
    Matrix4 getViewMatrix() const;

    /**
     * @brief Computes and returns the projection matrix of the camera.
     * 
     * @return The projection matrix.
     */
    Matrix4 getProjectionMatrix();

    /**
     * @brief Returns the cached projection matrix without recalculating it.
     *
     * @return The projection matrix.
     */
    Matrix4 getCachedProjectionMatrix() const;

    /**
     * @brief Gets the active camera projection implementation.
     * 
     * @return Pointer to the active ICameraProjection.
     */
    ICameraProjection* getCameraProjection();

    /**
     * @brief Gets the active camera projection implementation (const version).
     * 
     * @return Pointer to the active ICameraProjection.
     */
    const ICameraProjection* getCameraProjection() const;

    /**
     * @brief Gets the camera data block structure for GPU upload.
     *
     * @param transposedMatrices Whether to transpose matrices for GPU upload.
     *
     * @return The Camera data block structure.
     */
    dataBlockStructure::Camera getCameraDataBlockStructure(bool transposedMatrices) const;

  private:
    UUID m_id;
    Vector3f m_position;
    Vector3f m_direction;
    Vector3f m_up;
    projectionType::Type m_projectionType;
    PerspectiveCameraProjection m_perspectiveProjection;
    OrthographicCameraProjection m_orthographicProjection;

    void recalculateUp();
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Camera;

  /**
   * @brief Manages the lifecycle and storage of Camera objects in the engine.
   */
  class HC_CORE_EXPORT CameraManager :
    public NonCopyable,
    public io::ISerializable
  {
  public:
    CameraManager();
    ~CameraManager();

    /**
     * @brief Serializes the CameraManager and all managed cameras to binary format.
     *
     * Writes the number of cameras, followed by the serialized data of each
     * camera, including the default camera and the active camera's ID.
     * 
     * @param writer The BinaryWriter to use for serialization.
     */
    void serialize(io::BinaryWriter& writer) const override;
    
    /**
     * @brief Deserializes the CameraManager and all managed cameras from binary format.
     *
     * Reads the number of cameras, followed by the serialized data of each
     * camera, including the default camera and the active camera's ID.
     * 
     * @param reader The BinaryReader to use for deserialization.
     */
    void deserialize(io::BinaryReader& reader) override;

    /**
     * @brief Creates a new Camera.
     * 
     * The created Camera is owned and managed by the CameraManager, and will be
     * automatically destroyed when the CameraManager is destroyed or when clear() is
     * called. The caller should not delete the returned Camera pointer.
     * 
     * @return Pointer to the created Camera.
     */
    Camera* createCamera();

    /**
     * @brief Destroys a camera with the specified ID.
     *
     * Removes and destroys the camera from the managed cameras. Logs an
     * error if the camera is not found. If the active camera is destroyed,
     * the active camera is reset to the default camera.
     * 
     * @param cameraId The UUID of the camera to destroy.
     */
    void destroyCamera(const UUID& cameraId);

    /**
     * @brief Sets the active camera by ID.
     *
     * Logs an error and leaves the active camera unchanged if the camera
     * with the specified ID is not found.
     * 
     * @param cameraId The UUID of the camera to set as active.
     */
    void setActiveCamera(const UUID& cameraId);

    /**
     * @brief Gets a camera by its ID.
     * 
     * @param cameraId The UUID of the camera to retrieve.
     * 
     * @return Pointer to the camera, or nullptr if not found.
     */
    Camera* getCamera(const UUID& cameraId) const;

    /**
     * @brief Gets the active Camera.
     */
    Camera* getActiveCamera() const;

    /**
     * @brief Gets the default Camera.
     */
    Camera& getDefaultCamera() const;

    /**
     * @brief Removes and destroys all managed cameras.
     */
    void clear();

    /**
     * @brief Gets a list of all managed cameras.

     * The list will be filled with pointers to the cameras, but the caller @b does
     * @b not take ownership of the cameras.
     * 
     * @param outCameras Vector to be filled with pointers to all managed cameras.
     */
    void getCameras(Vector<Camera*>& outCameras) const;

  private:
    UnorderedMap<UUID, UniquePtr<Camera>> m_cameras;
    UniquePtr<Camera> m_default;
    Camera* m_activeCamera;
  };
}

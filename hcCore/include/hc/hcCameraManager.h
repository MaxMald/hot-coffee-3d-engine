#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Camera;

  /**
   * @brief Manages the lifecycle and storage of Camera objects in the engine.
   */
  class HC_CORE_EXPORT CameraManager :
    public NonCopyable
  {
  public:
    CameraManager();
    ~CameraManager();

    /**
     * @brief Creates a new Camera.
     * 
     * @return Pointer to the created Camera.
     */
    Camera* createCamera();

    /**
     * @brief Destroys a managed Camera.
     * 
     * @param camera Pointer to the Camera to destroy.
     */
    void destroyCamera(Camera* camera);

    /**
     * @brief Removes and destroys all managed cameras.
     */
    void clear();

    /**
     * @brief Gets the list of all managed cameras.
     * 
     * @return A const reference to the vector of unique pointers to Camera objects.
     */
    const Vector<UniquePtr<Camera>>& getCameras() const;

  private:
    Vector<UniquePtr<Camera>> m_cameras;
  };
}

#include "hc/hcCameraManager.h"
#include "hc/hcCamera.h"

namespace hc
{
  CameraManager::CameraManager() :
    m_activeCamera(nullptr)
  {
  }

  CameraManager::~CameraManager()
  {
  }

  Camera* CameraManager::createCamera()
  {
    UniquePtr<Camera> camera = UniquePtr<Camera>(new Camera());
    Camera* cameraPtr = camera.get();
    m_cameras.push_back(std::move(camera));
    return cameraPtr;
  }

  void CameraManager::destroyCamera(Camera* camera)
  {
    if (!camera)
      return;

    auto it = std::find_if(
      m_cameras.begin(),
      m_cameras.end(),
      [camera](const UniquePtr<Camera>& ptr) { return ptr.get() == camera; }
    );

    if (it != m_cameras.end())
    {
      m_cameras.erase(it, m_cameras.end());
    }
  }

  void CameraManager::setActiveCamera(Camera* camera)
  {
    m_activeCamera = camera;
  }

  void CameraManager::clear()
  {
    m_cameras.clear();
  }

  const Vector<UniquePtr<Camera>>& CameraManager::getCameras() const
  {
    return m_cameras;
  }
}

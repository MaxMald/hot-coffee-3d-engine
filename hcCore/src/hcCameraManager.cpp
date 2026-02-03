#include "hc/hcCameraManager.h"
#include "hc/hcCamera.h"

namespace hc
{
  CameraManager::CameraManager() :
    m_activeCamera(nullptr),
    m_default(MakeUnique<Camera>(new Camera()))
  {
    m_activeCamera = m_default.get();
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
    if (!camera)
      m_activeCamera = m_default.get();
    else
      m_activeCamera = camera;
  }

  Camera* CameraManager::getActiveCamera() const
  {
    return m_activeCamera;
  }

  void CameraManager::clear()
  {
    m_cameras.clear();
    m_activeCamera = m_default.get();
  }

  const Vector<UniquePtr<Camera>>& CameraManager::getCameras() const
  {
    return m_cameras;
  }
}

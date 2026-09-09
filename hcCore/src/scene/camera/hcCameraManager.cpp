#include "hc/scene/camera/hcCameraManager.h"
#include "hc/scene/camera/hcCamera.h"

namespace hc
{
  static constexpr UInt32 CAMERA_MANAGER_VERSION = 1;

  CameraManager::CameraManager() :
    m_cameras(),
    m_default(new Camera()),
    m_activeCamera(m_default.get())
  {
  }

  CameraManager::~CameraManager()
  {
  }

  void CameraManager::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(static_cast<UInt32>(0), CAMERA_MANAGER_VERSION);

    m_default->serialize(writer);

    writer.writeUInt32(static_cast<uint32_t>(m_cameras.size()));
    for (const auto& pair : m_cameras)
      pair.second->serialize(writer);

    // Serialize active camera ID
    if (m_activeCamera)
      writer.writeUUID(m_activeCamera->getUUID());
    else
      writer.writeUUID(m_default->getUUID());

    writer.finishWritingObject();
  }

  void CameraManager::deserialize(io::BinaryReader& reader)
  {
    m_cameras.clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(CAMERA_MANAGER_VERSION))
    {
      setActiveCamera(m_default->getUUID());
      reader.finishReadingObject();
      return;
    }

    m_default->deserialize(reader);

    uint32_t cameraCount = reader.readUInt32();
    for (uint32_t i = 0; i < cameraCount; ++i)
    {
      Camera* cameraPtr = new Camera();
      UniquePtr<Camera> camera(cameraPtr);
      cameraPtr->deserialize(reader);
      m_cameras.emplace(cameraPtr->getUUID(), std::move(camera));
    }

    // Deserialize active camera ID
    UUID activeCameraId = reader.readUUID();
    setActiveCamera(activeCameraId);

    reader.finishReadingObject();
  }

  Camera* CameraManager::createCamera()
  {
    Camera* cameraPtr = new Camera();
    UniquePtr<Camera> camera(cameraPtr);
    m_cameras.emplace(cameraPtr->getUUID(), std::move(camera));
    return cameraPtr;
  }

  void CameraManager::destroyCamera(const UUID& cameraId)
  {
    auto it = m_cameras.find(cameraId);
    if (it != m_cameras.end())
    {
      if (m_activeCamera == it->second.get())
        m_activeCamera = m_default.get();

      m_cameras.erase(it);
      return;
    }

    LogService::Error("Camera with ID " + cameraId.toString() + " was not found.");
  }

  Camera* CameraManager::getCamera(const UUID& cameraId) const
  {
    if (cameraId == m_default->getUUID())
      return m_default.get();

    auto it = m_cameras.find(cameraId);
    if (it != m_cameras.end())
      return it->second.get();

    return nullptr;
  }

  void CameraManager::setActiveCamera(const UUID& cameraId)
  {
    Camera* camera = getCamera(cameraId);
    if (!camera)
    {
      LogService::Error("Camera with ID " + cameraId.toString() + " was not found. Active camera remains unchanged.");
      return;
    }

    m_activeCamera = camera;
  }

  Camera* CameraManager::getActiveCamera() const
  {
    return m_activeCamera;
  }

  Camera& CameraManager::getDefaultCamera() const
  {
    return *m_default;
  }

  void CameraManager::clear()
  {
    m_cameras.clear();
    m_activeCamera = m_default.get();
  }

  void CameraManager::getCameras(Vector<Camera*>& outCameras) const
  {
    outCameras.clear();
    for (const auto& pair : m_cameras)
      outCameras.push_back(pair.second.get());
  }
}

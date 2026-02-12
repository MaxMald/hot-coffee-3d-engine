#include "hc/hcCameraComponentFactory.h"

namespace hc
{
  CameraComponentFactory::CameraComponentFactory(SceneManager& sceneManager) :
    m_sceneManager(sceneManager)
  {
  }

  UniquePtr<CameraComponent> CameraComponentFactory::createComponent() const
  {
    return MakeUnique<CameraComponent>(m_sceneManager);
  }
}

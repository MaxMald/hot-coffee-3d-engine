#include "hc/scene/gameObject/components/factories/hcCameraComponentFactory.h"
#include "hc/scene/hcSceneManager.h"

namespace hc
{
  CameraComponentFactory::CameraComponentFactory(SceneManager& sceneManager) :
    ATypedComponentFactory<CameraComponent>(),
    m_sceneManager(sceneManager)
  {
  }

  CameraComponentFactory::~CameraComponentFactory()
  {
  }

  UniquePtr<CameraComponent> CameraComponentFactory::create() const
  {
    return MakeUnique<CameraComponent>(m_sceneManager);
  }
}

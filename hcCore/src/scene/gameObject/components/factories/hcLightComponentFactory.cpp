#include "hc/scene/gameObject/components/factories/hcLightComponentFactory.h"
#include "hc/scene/gameObject/components/hcLightComponent.h"
#include "hc/scene/light/hcLightManager.h"
#include "hc/scene/hcSceneManager.h"

namespace hc
{
  LightComponentFactory::LightComponentFactory(SceneManager& sceneManager) :
    m_sceneManager(sceneManager)
  {
  }

  UniquePtr<LightComponent> LightComponentFactory::create() const
  {
    Scene* activeScene = m_sceneManager.getActiveScene();
    if (!activeScene)
    {
      LogService::Error("Cannot create LightComponent: No active scene in SceneManager.");
      return nullptr;
    }

    return MakeUnique<LightComponent>(activeScene->getLightManager());
  }
}

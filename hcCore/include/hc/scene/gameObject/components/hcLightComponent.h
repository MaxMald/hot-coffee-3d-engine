#pragma once

#include "hc/scene/gameObject/components/hcABaseComponent.h"

namespace hc
{
  class Light;
  class LightManager;

  class HC_CORE_EXPORT LightComponent : public ABaseComponent
  {
  public:
    LightComponent(LightManager& lightManager);
    ~LightComponent() override;

    void create();
    Light* getLight() const;
    bool hasLight() const;
    void destroyLight();

  private:
    Light* m_light;
    LightManager& m_lightManager;

    /**
     * @copydoc ABaseComponent::onSerialize
     */
    void onSerialize(BinaryWriter& writer) const override;

    /**
     * @copydoc ABaseComponent::onDeserialize
     */
    void onDeserialize(BinaryReader& reader) override;
  };
}

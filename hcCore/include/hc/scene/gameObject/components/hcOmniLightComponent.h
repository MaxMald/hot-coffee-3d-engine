#pragma once

#include "hc/scene/light/hcOmniLight.h"
#include "hc/scene/gameObject/components/hcALightComponent.h"

namespace hc
{
  class HC_CORE_EXPORT OmniLightComponent : public ALightComponent
  {
  public:
    OmniLightComponent();

    virtual ~OmniLightComponent() override;

    void serialize(io::BinaryWriter& writer) const override;

    void deserialize(io::BinaryReader& reader) override;

    void preUpdate(float deltaTime) override;

    void update(float deltaTime) override;

    void postUpdate(float deltaTime) override;

    ALight& getLight() override;

    const ALight& getLight() const override;

    void updateLight() override;

    OmniLight& getOmniLight();

    const OmniLight& getOmniLight() const;

  private:
    OmniLight m_omniLight;
  };
}

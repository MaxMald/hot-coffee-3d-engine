#pragma once

#include "hc/scene/light/hcSpotLight.h"
#include "hc/scene/gameObject/components/hcALightComponent.h"

namespace hc
{
  class HC_CORE_EXPORT SpotLightComponent : public ALightComponent
  {
  public:
    SpotLightComponent();

    virtual ~SpotLightComponent() override;

    void serialize(io::BinaryWriter& writer) const override;

    void deserialize(io::BinaryReader& reader) override;

    void preUpdate(float deltaTime) override;

    void update(float deltaTime) override;

    void postUpdate(float deltaTime) override;

    ALight& getLight() override;

    const ALight& getLight() const override;

    void updateLight() override;

    SpotLight& getSpotLight();

    const SpotLight& getSpotLight() const;

  private:
    SpotLight m_spotLight;
  };
}

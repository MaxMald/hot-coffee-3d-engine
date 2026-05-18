#pragma once

#include "hc/scene/light/hcDirectionalLight.h"
#include "hc/scene/gameObject/components/hcALightComponent.h"

namespace hc
{
  class HC_CORE_EXPORT DirectionalLightComponent : public ALightComponent
  {
  public:
    DirectionalLightComponent();

    virtual ~DirectionalLightComponent() override;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    void preUpdate(float deltaTime) override;

    void update(float deltaTime) override;

    void postUpdate(float deltaTime) override;

    ALight& getLight() override;

    const ALight& getLight() const override;

    void updateLight() override;

    DirectionalLight& getDirectionalLight();

    const DirectionalLight& getDirectionalLight() const;

  private:
    DirectionalLight m_directionalLight;
  };
}

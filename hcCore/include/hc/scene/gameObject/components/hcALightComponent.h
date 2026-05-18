#pragma once

#include "hc/scene/gameObject/components/hcABaseComponent.h"
#include "hc/scene/gameObject/components/hcIUpdatableComponent.h"

namespace hc
{
  class ALight;

  class HC_CORE_EXPORT ALightComponent :
    public ABaseComponent,
    public IUpdatableComponent
  {
  public:
    virtual ~ALightComponent() override = default;

    virtual ALight& getLight() = 0;

    virtual const ALight& getLight() const = 0;

    virtual void updateLight() = 0;

  protected:
    ALightComponent(componentType::Type type);
  };
}

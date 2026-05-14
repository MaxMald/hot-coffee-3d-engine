#pragma once

#include "hc/scene/light/hcALight.h"

namespace hc
{
  class HC_CORE_EXPORT OmniLight : public ALight
  {
  public:
    OmniLight();

    ~OmniLight() override = default;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    void setRange(float range);

    float getRange() const;

  private:
    float m_range;
  };
}

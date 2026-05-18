#pragma once

#include "hc/scene/light/hcALight.h"

namespace hc
{
  class HC_CORE_EXPORT SpotLight : public ALight
  {
  public:
    SpotLight();

    ~SpotLight() override;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    void setDirection(const Vector3f& direction);

    const Vector3f& getDirection() const;

    void setInnerConeAngle(Angle angle);

    Angle getInnerConeAngle() const;

    void setOuterConeAngle(Angle angle);

    Angle getOuterConeAngle() const;

  private:
    Vector3f m_direction;
    Angle m_innerConeAngle;
    Angle m_outerConeAngle;
  };
}

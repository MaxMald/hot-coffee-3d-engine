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

    void setInnerConeAngle(float angle);

    float getInnerConeAngle() const;

    void setOuterConeAngle(float angle);

    float getOuterConeAngle() const;

  private:
    Vector3f m_direction;
    float m_innerConeAngle;
    float m_outerConeAngle;
  };
}

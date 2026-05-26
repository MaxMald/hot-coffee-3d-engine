#pragma once

#include "hc/scene/light/hcALight.h"
#include "hc/graphics/lightFrameData/hcDirectionalLightFrameData.h"

namespace hc
{
  class HC_CORE_EXPORT DirectionalLight : public ALight
  {
  public:
    DirectionalLight();

    ~DirectionalLight() override;

    void serialize(BinaryWriter& writer) const override;

    void deserialize(BinaryReader& reader) override;

    void setDirection(const Vector3f& direction);

    const Vector3f& getDirection() const;

    DirectionalLightFrameData toFrameData() const;

  private:
    Vector3f m_direction;
  };
}

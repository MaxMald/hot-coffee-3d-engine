#pragma once

#include "hc/serialization/hcIJsonSerializer.h"

namespace hc
{
  class AMaterialDescriptor;
  class UnlitMaterialDescriptor;
  class JsonObjectBuilder;
  class Json;

  class MaterialDescriptorJsonSerializer :
    public IJsonSerializer<AMaterialDescriptor>
  {
  public:
    MaterialDescriptorJsonSerializer() = default;
    virtual ~MaterialDescriptorJsonSerializer() = default;

    Optional<String> serialize(
      const AMaterialDescriptor* data
    ) const override;

    AMaterialDescriptor* deserialize(
      const String& jsonString
    ) const override;

  private:
    bool serializeUnlitMaterialDescriptor(
      const UnlitMaterialDescriptor* data,
      JsonObjectBuilder& objectBuilder
    ) const;

    UnlitMaterialDescriptor* deserializeUnlitMaterialDescriptor(
      const Json& json
    ) const;
  };
}

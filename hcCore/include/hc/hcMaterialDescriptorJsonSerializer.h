#pragma once

#include "hc/hcIJsonSerializer.h"

namespace hc
{
  class MaterialDescriptor;
  class UnlitMaterialDescriptor;
  class JsonObjectBuilder;
  class Json;

  class MaterialDescriptorJsonSerializer :
    public IJsonSerializer<MaterialDescriptor>
  {
  public:
    MaterialDescriptorJsonSerializer() = default;
    virtual ~MaterialDescriptorJsonSerializer() = default;

    Optional<String> serialize(
      const MaterialDescriptor* data
    ) const override;

    MaterialDescriptor* deserialize(
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

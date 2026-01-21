#include "hc/hcMaterialDescriptorJsonSerializer.h"
#include "hc/hcMaterialDescriptor.h"
#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcJsonObjectBuilder.h"
#include "hc/hcJson.h"
#include "hc/hcJsonParsers.h"

namespace hc
{
  Optional<String> MaterialDescriptorJsonSerializer::serialize(
    const MaterialDescriptor* data
  ) const
  {
    if (data == nullptr)
      return {};

    JsonObjectBuilder builder;

    String shaderTypeStr = shaderType::toString(data->getShaderType());
    builder.setString("shaderType", shaderTypeStr);

    switch (data->getShaderType())
    {
    case shaderType::Type::Unlit:
    {
      const UnlitMaterialDescriptor* unlitMaterialDescriptor =
        static_cast<const UnlitMaterialDescriptor*>(data);

      if (!serializeUnlitMaterialDescriptor(unlitMaterialDescriptor, builder))
        return {};

      break;
    }
    default:
      return {};
    }

    return builder.toString();
  }

  MaterialDescriptor* MaterialDescriptorJsonSerializer::deserialize(
    const String& jsonString
  ) const
  {
    Json json = Json::parse(jsonString);
    if (json.isNull())
      return nullptr;

    String shaderTypeStr = json["shaderType"].getString();
    shaderType::Type shaderType = shaderType::fromString(shaderTypeStr);

    switch (shaderType)
    {
    case shaderType::Unlit:
      return deserializeUnlitMaterialDescriptor(json);
    default:
      return nullptr;
    }
  }

  bool MaterialDescriptorJsonSerializer::serializeUnlitMaterialDescriptor(
    const UnlitMaterialDescriptor* data,
    JsonObjectBuilder& objectBuilder
  ) const
  {
    if (data == nullptr)
      return false;

    objectBuilder.setColor("color", data->getColor());
    objectBuilder.setString("mainImageKey", data->getMainImageKey());
    return true;
  }

  UnlitMaterialDescriptor*
    MaterialDescriptorJsonSerializer::deserializeUnlitMaterialDescriptor(
    const Json& json
  ) const
  {
    UnlitMaterialDescriptor* descriptor = new UnlitMaterialDescriptor();

    try
    {
      Color color = jsonParsers::parseColor(json["color"]);
      descriptor->setColor(color);
      String mainImageKey = json["mainImageKey"].getString();
      descriptor->setMainImageKey(mainImageKey);
    }
    catch (...)
    {
      delete descriptor;
      return nullptr;
    }

    return descriptor;
  }
}

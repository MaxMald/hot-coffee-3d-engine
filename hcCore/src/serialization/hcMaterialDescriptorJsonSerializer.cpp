#include "hc/serialization/hcMaterialDescriptorJsonSerializer.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"
#include "hc/assets/materialDescriptor/hcUnlitMaterialDescriptor.h"

namespace hc
{
  Optional<String> MaterialDescriptorJsonSerializer::serialize(
    const AMaterialDescriptor* data
  ) const
  {
    if (data == nullptr)
      return {};

    JsonObjectBuilder builder;

    String shaderTypeStr = shadingType::toString(data->getShaderType());
    builder.setString("shaderType", shaderTypeStr);

    switch (data->getShaderType())
    {
    case shadingType::Type::Unlit:
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

  AMaterialDescriptor* MaterialDescriptorJsonSerializer::deserialize(
    const String& jsonString
  ) const
  {
    Json json = Json::parse(jsonString);
    if (json.isNull())
      return nullptr;

    String shaderTypeStr = json["shaderType"].getString();
    shadingType::Type shaderType = shadingType::fromString(shaderTypeStr);

    switch (shaderType)
    {
    case shadingType::Unlit:
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
    objectBuilder.setPath("mainImagePath", data->getMainImagePath());
    return true;
  }

  UnlitMaterialDescriptor*
    MaterialDescriptorJsonSerializer::deserializeUnlitMaterialDescriptor(
    const Json& json
  ) const
  {
    try
    {
      Color color = jsonParsers::parseColor(json["color"]);
      String mainImagePathStr = json["mainImagePath"].getString();
      Path mainImagePath(mainImagePathStr.c_str());
      Path path = ""; // TODO should the path be included in the JSON? Or should it be set separately after deserialization?";

      return new UnlitMaterialDescriptor(
        path,
        color,
        mainImagePath
      );
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to deserialize UnlitMaterialDescriptor: %s",
          e.what()
        )
      );
      return nullptr;
    }
  }
}

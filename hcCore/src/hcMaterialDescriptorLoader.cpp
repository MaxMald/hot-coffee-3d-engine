#include "hc/hcMaterialDescriptorLoader.h"

#include "hc/hcUnlitMaterialDescriptor.h"
#include "hc/hcJson.h"
#include "hc/hcJsonParsers.h"

namespace hc
{
  MaterialDescriptorLoader::MaterialDescriptorLoader()
  {
  }

  MaterialDescriptorLoader::~MaterialDescriptorLoader()
  {
  }

  SharedPtr<MaterialDescriptor> MaterialDescriptorLoader::load(const Path& path)
  {
    Json json = Json::loadFromFile(path);
    if (json.isNull())
      return nullptr;

    String shaderTypeStr = json["shaderType"].getString();
    shaderType::Type shaderType = shaderType::fromString(shaderTypeStr);

    switch (shaderType)
    {
      case shaderType::Unlit:
        return loadUnlitMaterialDescriptor(json);
      default:
        return nullptr;
    }
  }

  SharedPtr<UnlitMaterialDescriptor> MaterialDescriptorLoader::loadUnlitMaterialDescriptor(
    const Json& json
  )
  {
    SharedPtr<UnlitMaterialDescriptor> descriptor =
      MakeShared<UnlitMaterialDescriptor>();

    Color color = jsonParsers::parseColor(json["color"]);
    descriptor->setColor(color);
    String mainImageKey = json["mainImageKey"].getString();
    descriptor->setMainImageKey(mainImageKey);

    return descriptor;
  }
}

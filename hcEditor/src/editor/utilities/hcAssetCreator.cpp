#include "hc/editor/utilities/hcAssetCreator.h"

namespace hc::editor
{
  namespace assetCreator
  {
    bool createMaterialDescriptor(const Path& assetPath)
    {
      UnlitMaterialDescriptor materialDesc(
        assetPath,
        "UnlitMaterial",
        Color(1.0f, 1.0f, 1.0f, 1.0f),
        ""
      );

      AMaterialDescriptor* materialDescPtr = &materialDesc;
      Optional<String> jsonStr = 
        JsonSerializer::Serialize<AMaterialDescriptor>(materialDescPtr);

      if (!jsonStr.has_value())
        return false;

      return fileUtilities::SaveStringToFile(assetPath, jsonStr.value());
    }
  }
}

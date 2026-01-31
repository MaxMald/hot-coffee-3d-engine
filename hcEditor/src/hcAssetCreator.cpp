#include "hc/editor/hcAssetCreator.h"

namespace hc::editor
{
  namespace assetCreator
  {
    bool createMaterialDescriptor(const Path& assetPath)
    {
      UnlitMaterialDescriptor materialDesc;
      MaterialDescriptor* materialDescPtr = &materialDesc;
      Optional<String> jsonStr = 
        JsonSerializer::Serialize<MaterialDescriptor>(materialDescPtr);

      if (!jsonStr.has_value())
        return false;

      return fileUtilities::SaveStringToFile(assetPath, jsonStr.value());
    }
  }
}

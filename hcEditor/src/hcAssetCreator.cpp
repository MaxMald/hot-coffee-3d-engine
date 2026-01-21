#include "hc/editor/hcAssetCreator.h"

#include <hc/hcJsonSerializer.h>
#include <hc/hcUnlitMaterialDescriptor.h>
#include <hc/hcFileUtilities.h>

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

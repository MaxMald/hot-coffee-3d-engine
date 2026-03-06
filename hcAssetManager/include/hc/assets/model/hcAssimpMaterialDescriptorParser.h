#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

struct aiMaterial;

namespace hc
{
  struct AssimpMaterialDescriptorParser
  {
  public:
    static SharedPtr<AMaterialDescriptor> Parse(
      const Path& fileDirectory,
      const aiMaterial* material
    );

  private:
    static shadingType::Type DetermineShadingType(const aiMaterial* material);
    static SharedPtr<AMaterialDescriptor> ParseUnlitMaterialDescriptor(
      const Path& fileDirectory,
      const aiMaterial* material
    );
  };
}

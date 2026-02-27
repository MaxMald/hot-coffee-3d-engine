#pragma once

#include "hc/hcAssetManagerPrerequisites.h"

struct aiMaterial;

namespace hc
{
  namespace materialParser
  {
    SharedPtr<AMaterialDescriptor> ParseMaterialDescriptorFromAssimpMaterial(
      const Path& fileDirectory,
      const aiMaterial* material
    );

    shadingType::Type DetermineShadingType(const aiMaterial* material);

    SharedPtr<AMaterialDescriptor> CreateDefaultMaterial();

    SharedPtr<AMaterialDescriptor> ParseUnlitMaterialDescriptor(
      const Path& fileDirectory,
      const aiMaterial* material
    );
  }
}

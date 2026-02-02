#pragma once

#include "hc/hcAssetLoadersPrerequisites.h"

struct aiMaterial;

namespace hc
{
  namespace materialParser
  {
    SharedPtr<MaterialDescriptor> ParseMaterialDescriptorFromAssimpMaterial(const aiMaterial* material);
    shadingType::Type DetermineShadingType(const aiMaterial* material);
    SharedPtr<MaterialDescriptor> CreateDefaultMaterial();
    SharedPtr<MaterialDescriptor> ParseUnlitMaterialDescriptor(const aiMaterial* material);
  }
}

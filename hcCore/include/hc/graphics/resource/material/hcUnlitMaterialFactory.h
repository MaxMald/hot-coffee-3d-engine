#pragma once

#include "hc/graphics/resource/material/hcIMaterialFactory.h"

namespace hc
{
  /**
   * @brief Factory for creating unlit materials.
   */
  class UnlitMaterialFactory : public IMaterialFactory
  {
  public:
    UnlitMaterialFactory();
    ~UnlitMaterialFactory() override = default;

    /**
     * @brief Creates an unlit material instance.
     *
     * @param materialId The unique identifier for the material, used for sorting and
     * batching.
     * @param descriptor Shared pointer to the material descriptor.
     * @param textureManager Reference to the texture manager used for material
     * creation.
     * @param shaderProgramManager Reference to the shader program manager used
     * for material creation.
     * 
     * @return Shared pointer to the created IMaterial instance. nullptr if the
     * creation failed.
     */
    SharedPtr<IMaterial> create(
      UInt16 materialId,
      SharedPtr<AMaterialDescriptor> descriptor,
      ITextureManager& textureManager,
      IShaderProgramManager& shaderProgramManager
    ) override;
  };
}

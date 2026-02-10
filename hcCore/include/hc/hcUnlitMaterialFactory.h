#pragma once

#include "hc/hcIMaterialFactory.h"

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
      SharedPtr<MaterialDescriptor> descriptor,
      ITextureManager& textureManager,
      IShaderProgramManager& shaderProgramManager
    ) override;
  };
}

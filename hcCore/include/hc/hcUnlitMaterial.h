#pragma once

#include "hc/hcIMaterial.h"

namespace hc
{
  class UnlitMaterialDescriptor;
  class ITexture;

  /**
   * @brief Represents a material that is rendered without lighting effects.
   */
  class UnlitMaterial : public IMaterial
  {
  public:
    UnlitMaterial();
    ~UnlitMaterial() override;

    /**
     * @copydoc IMaterial::getShaderType
     */
    shaderType::Type getShaderType() const override;

    /**
     * @brief Initializes the unlit material with a descriptor and main texture.
     *
     * @param descriptor Shared pointer to the unlit material descriptor.
     * @param mainTexture Shared pointer to the main texture (can be nullptr).
     */
    void initialize(
      const SharedPtr<UnlitMaterialDescriptor>& descriptor,
      const SharedPtr<ITexture>& mainTexture
    );

    /**
     * @brief Gets the color property of the material.
     * 
     * @return The color used by the material.
     */
    const Color& getColor() const;

    /**
     * @brief Gets the main texture used by the material.
     * 
     * @return Shared pointer to the main texture, or nullptr if not set.
     */
    const SharedPtr<ITexture>& getMainTexture() const;

    /**
     * @brief Gets the descriptor associated with this material.
     * 
     * @return Shared pointer to the unlit material descriptor.
     */
    const SharedPtr<UnlitMaterialDescriptor> getDescriptor() const;

  private:
    SharedPtr<UnlitMaterialDescriptor> m_descriptor;
    SharedPtr<ITexture> m_mainTexture;
  };
}

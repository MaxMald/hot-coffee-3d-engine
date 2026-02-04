#pragma once

#include "hc/hcIMaterial.h"

namespace hc
{
  class UnlitMaterialDescriptor;
  class ITexture;
  class IShaderProgram;

  /**
   * @brief Represents a material that is rendered without lighting effects.
   */
  class HC_CORE_EXPORT UnlitMaterial : public IMaterial
  {
  public:
    UnlitMaterial();
    ~UnlitMaterial() override;

    /**
     * @copydoc IMaterial::getShaderType
     */
    shadingType::Type getShaderType() const override;

    /**
     * @copydoc IMaterial::bind
     */
    void bind(const CameraMatrices& cameraMatrices) override;

    /**
     * @copydoc IMaterial::updateModelMatrix
     */
    void updateModelMatrix(const Matrix4& modelMatrix) override;

    /**
     * @copydoc IMaterial::unbind
     */
    void unbind() override;

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
    SharedPtr<IShaderProgram> m_shaderProgram;
    SharedPtr<ITexture> m_mainTexture;
  };
}

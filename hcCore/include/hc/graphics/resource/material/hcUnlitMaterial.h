#pragma once

#include "hc/graphics/resource/material/hcIMaterial.h"

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
     * @copydoc IMaterial::getId
     */
    const Id& getId() const override;

    /**
     * @copydoc IMaterial::destroy
     */
    void destroy() override;

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
     * @copydoc IMaterial::getDescriptor
     */
    SharedPtr<MaterialDescriptor> getDescriptor() const override;

    /**
     * @brief Initializes the unlit material with a descriptor and main texture.
     *
     * @param descriptor Shared pointer to the unlit material descriptor.
     * @param mainTexture Shared pointer to the main texture (can be nullptr).
     */
    void initialize(
      const SharedPtr<IShaderProgram>& shaderProgram,
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

  private:
    Id m_id;
    SharedPtr<UnlitMaterialDescriptor> m_descriptor;
    SharedPtr<IShaderProgram> m_shaderProgram;
    SharedPtr<ITexture> m_mainTexture;
  };
}

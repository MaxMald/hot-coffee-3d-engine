#pragma once

#include "hc/graphics/resource/material/hcAMaterial.h"

namespace hc
{
  class UnlitMaterialDescriptor;
  class ITexture;
  class IShaderProgram;

  /**
   * @brief Represents a material that is rendered without lighting effects.
   */
  class HC_CORE_EXPORT UnlitMaterial : public AMaterial
  {
  public:
    /**
     * @brief Constructs an unlit material with the specified material ID.
     *
     * @param materialId The unique identifier for this material, used for sorting and
     * batching.
     */
    UnlitMaterial(UInt16 materialId);
    ~UnlitMaterial() override;

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
    void bind(const CameraRenderData& cameraRenderData, renderPassType::Type renderPass) override;

    /**
     * @copydoc IMaterial::updateModelMatrix
     */
    void updateModelMatrix(const Matrix4& modelMatrix) override;

    /**
     * @copydoc IMaterial::unbind
     */
    void unbind() override;

    /**
     * @copydoc IMaterial::isValid
     */
    bool isValid() const override;

    /**
     * @brief Initializes the unlit material with a descriptor and main texture.
     *
     * @param descriptor Reference to the unlit material descriptor.
     * @param shaderProgram Shared pointer to the shader program used for rendering the
     * material.
     * @param mainTexture Shared pointer to the main texture (can be nullptr).
     */
    void initialize(
      const UnlitMaterialDescriptor& descriptor,
      const SharedPtr<IShaderProgram>& shaderProgram,
      const SharedPtr<ITexture>& mainTexture
    );

    /**
     * @brief Gets the color property of the material.
     *
     * @return The color used by the material.
     */
    const Color& getColor() const;

    /**
     * @brief Sets the color property of the material.
     *
     * @param color The new color to use for the material.
     */
    void setColor(const Color& color);

    /**
     * @brief Gets the main texture used by the material.
     *
     * @return Shared pointer to the main texture, or nullptr if not set.
     */
    const SharedPtr<ITexture>& getMainTexture() const;

    /**
     * @brief Sets the main texture for the material.
     *
     * @param mainTexture Shared pointer to the new main texture (can be nullptr).
     */
    void setMainTexture(const SharedPtr<ITexture>& mainTexture);

  private:
    Color m_color;
    SharedPtr<IShaderProgram> m_shaderProgram;
    SharedPtr<ITexture> m_mainTexture;
  };
}

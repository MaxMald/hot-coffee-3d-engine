#pragma once

#include "hc/graphics/resource/material/hcAMaterial.h"

namespace hc
{
  class BlinnPhongMaterialDescriptor;
  class ITexture;
  class IShaderProgram;

  /**
   * @brief Represents a material that uses the Blinn-Phong shading model for rendering.
   */
  class HC_CORE_EXPORT BlinnPhongMaterial : public AMaterial
  {
  public:
    /**
     * @brief Constructs a Blinn-Phong material with the specified material ID.
     *
     * @param materialId The unique identifier for this material, used for sorting and
     * batching.
     */
    BlinnPhongMaterial(UInt16 materialId);
    ~BlinnPhongMaterial() override;

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
    void bind(
      renderPassType::Type renderPass,
      IDataBlockManager& dataBlockManager
    ) override;

    /**
     * @copydoc IMaterial::unbind
     */
    void unbind() override;

    /**
     * @copydoc IMaterial::isValid
     */
    bool isValid() const override;

    /**
     * @brief Initializes the Blinn-Phong material with the provided descriptor and
     * associated resources.
     *
     * This method sets up the material's properties and resources based on the given
     * descriptor and texture/shader references. It should be called after constructing
     * the material and before using it for rendering.
     *
     * @param descriptor The material descriptor containing properties and resource paths.
     * @param albedoTexture Shared pointer to the albedo texture resource.
     * @param normalTexture Shared pointer to the normal map texture resource.
     * @param specularTexture Shared pointer to the specular texture resource.
     * @param forwardShaderProgram Shared pointer to the shader program used for forward rendering this material.
     * @param deferredGeometryShaderProgram Shared pointer to the shader program used for deferred geometry rendering this material.
     */
    void initialize(
      const BlinnPhongMaterialDescriptor& descriptor,
      const SharedPtr<ITexture>& albedoTexture,
      const SharedPtr<ITexture>& normalTexture,
      const SharedPtr<ITexture>& specularTexture,
      const SharedPtr<IShaderProgram>& forwardShaderProgram,
      const SharedPtr<IShaderProgram>& deferredGeometryShaderProgram
    );

    /**
     * @brief Gets the base color of the material.
     *
     * @return Reference to the base color of the material.
     */
    const Color& getColor() const;

    /**
     * @brief Sets the base color of the material.
     *
     * @param color The base color to set for the material.
     */
    void setColor(const Color& color);

    /**
     * @brief Gets the shininess of the material, which controls the size of the
     * specular highlight.
     *
     * @return The shininess value of the material.
     */
    float getShininess() const;

    /**
     * @brief Sets the shininess of the material, which controls the size of the
     * specular highlight. The value is clamped between 1.0 and 256.0.
     *
     * @param shininess The shininess value to set for the material.
     */
    void setShininess(float shininess);

    /**
     * @brief Gets the albedo texture associated with the material.
     *
     * @return Shared pointer to the albedo texture resource.
     */
    const SharedPtr<ITexture>& getAlbedoTexture() const;

    /**
     * @brief Sets the albedo texture associated with the material.
     *
     * @param albedoTexture Shared pointer to the albedo texture resource.
     */
    void setAlbedoTexture(const SharedPtr<ITexture>& albedoTexture);

    /**
     * @brief Gets the normal map texture associated with the material.
     *
     * @return Shared pointer to the normal map texture resource.
     */
    const SharedPtr<ITexture>& getNormalTexture() const;

    /**
     * @brief Sets the normal map texture associated with the material.
     *
     * @param normalTexture Shared pointer to the normal map texture resource.
     */
    void setNormalTexture(const SharedPtr<ITexture>& normalTexture);

    /**
     * @brief Gets the specular texture associated with the material.
     *
     * @return Shared pointer to the specular texture resource.
     */
    const SharedPtr<ITexture>& getSpecularTexture() const;

    /**
     * @brief Sets the specular texture associated with the material.
     *
     * @param specularTexture Shared pointer to the specular texture resource.
     */
    void setSpecularTexture(const SharedPtr<ITexture>& specularTexture);

  private:
    Color m_color;
    float m_shininess;
    SharedPtr<ITexture> m_albedoTexture;
    SharedPtr<ITexture> m_normalTexture;
    SharedPtr<ITexture> m_specularTexture;

    SharedPtr<IShaderProgram> m_forwardShaderProgram;
    SharedPtr<IShaderProgram> m_deferredGeometryShaderProgram;

    void assertIsValid() const;
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcAMaterial.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"

namespace hc
{
  class ITexture;
  class IShaderProgram;

  /**
   * @brief Represents a material that uses the Physically Based Rendering (PBR) shading
   * model.
   */
  class HC_CORE_EXPORT PBRMaterial : public AMaterial
  {
  public:
    /**
     * @brief Constructs a PBR material with the specified material index.
     *
     * @param materialIndex The unique index for this material, used for sorting and
     * batching.
     */
    PBRMaterial(UInt16 materialIndex);
    virtual ~PBRMaterial() override;

    /**
     * @copydoc IMaterial::destroy
     */
    void destroy() override;

    /**
     * @copydoc IMaterial::getMaterialType
     */
    materialType::Type getMaterialType() const override;

    /**
     * @copydoc IMaterial::setTexture
     */
    void setTexture(textureType::Type type, const SharedPtr<ITexture>& texture) override;

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
     * @copydoc IMaterial::getSourcePath
     */
    inline const Path& getSourcePath() const
    {
      return m_sourcePath;
    }

    /**
     * @brief Initializes the PBR material with the provided descriptor and associated
     * textures.
     *
     * @param descriptor The material descriptor containing PBR properties.
     * @param albedoTexture The albedo texture for the material.
     * @param normalTexture The normal map texture for the material.
     * @param omrTexture The occlusion-metallic-roughness (OMR) texture for the material.
     * @param deferredGeometryShaderProgram The shader program used for deferred geometry
     * rendering of this material.
     * @param forwardShaderProgram The shader program used for forward rendering of this
     * material.
     */
    void initialize(
      const MaterialDescriptor& descriptor,
      const SharedPtr<ITexture>& albedoTexture,
      const SharedPtr<ITexture>& normalTexture,
      const SharedPtr<ITexture>& omrTexture,
      const SharedPtr<IShaderProgram>& deferredGeometryShaderProgram,
      const SharedPtr<IShaderProgram>& forwardShaderProgram
    );

    /**
     * @brief Gets the albedo texture associated with the PBR material.
     *
     * @return Shared pointer to the albedo texture resource.
     */
    inline const SharedPtr<ITexture>& getAlbedoTexture() const
    {
      return m_albedoTexture;
    }

    /**
     * @brief Sets the albedo texture associated with the PBR material.
     *
     * @param albedoTexture Shared pointer to the albedo texture resource.
     */
    inline void setAlbedoTexture(const SharedPtr<ITexture>& albedoTexture)
    {
      m_albedoTexture = albedoTexture;
    }

    /**
     * @brief Gets the normal map texture associated with the PBR material.
     *
     * @return Shared pointer to the normal map texture resource.
     */
    inline const SharedPtr<ITexture>& getNormalTexture() const
    {
      return m_normalTexture;
    }

    /**
     * @brief Sets the normal map texture associated with the PBR material.
     *
     * @param normalTexture Shared pointer to the normal map texture resource.
     */
    inline void setNormalTexture(const SharedPtr<ITexture>& normalTexture)
    {
      m_normalTexture = normalTexture;
    }

    /**
     * @brief Gets the occlusion-roughness-metallic (ORM) texture associated with the PBR
     * material.
     *
     * @return Shared pointer to the ORM texture resource.
     */
    inline const SharedPtr<ITexture>& getORMTexture() const
    {
      return m_ormTexture;
    }

    /**
     * @brief Sets the occlusion-roughness-metallic (ORM) texture associated with the PBR
     * material.
     *
     * @param ormTexture Shared pointer to the ORM texture resource.
     */
    inline void setORMTexture(const SharedPtr<ITexture>& ormTexture)
    {
      m_ormTexture = ormTexture;
    }

    /**
     * @brief Gets the base color of the PBR material.
     *
     * @return The base color as a Color object.
     */
    inline const Color& getBaseColor() const
    {
      return m_baseColor;
    }

    /**
     * @brief Sets the base color of the PBR material.
     *
     * @param color The base color to set for the material.
     */
    inline void setBaseColor(const Color& color)
    {
      m_baseColor = color;
    }

    /**
     * @brief Gets the metallic factor of the PBR material.
     *
     * @return The metallic factor as a float in the range [0.0, 1.0].
     */
    inline float getMetallic() const
    {
      return m_metallic;
    }

    /**
     * @brief Sets the metallic factor of the PBR material.
     *
     * @param metallic The metallic factor to set for the material, clamped to the range
     * [0.0, 1.0].
     */
    inline void setMetallic(float metallic)
    {
      m_metallic = Math::Max(0.0f, Math::Min(1.0f, metallic));
    }

    /**
     * @brief Gets the roughness factor of the PBR material.
     *
     * @return The roughness factor as a float in the range [0.0, 1.0].
     */
    inline float getRoughness() const
    {
      return m_roughness;
    }

    /**
     * @brief Sets the roughness factor of the PBR material.
     *
     * @param roughness The roughness factor to set for the material, clamped to the
     * range [0.0, 1.0].
     */
    inline void setRoughness(float roughness)
    {
      m_roughness = Math::Max(0.0f, Math::Min(1.0f, roughness));
    }
    
    /**
     * @brief Gets the index of refraction (IOR) of the PBR material.
     *
     * @return The index of refraction as a float.
     */
    inline float getIOR() const
    {
      return m_ior;
    }

    /**
     * @brief Sets the index of refraction (IOR) of the PBR material.
     *
     * @param ior The index of refraction to set for the material, clamped to a minimum
     * value of Math::Epsilon.
     */
    inline void setIOR(float ior)
    {
      m_ior = Math::Clamp(ior, Math::Epsilon, assets::materialDescriptor::MAXIMUM_IOR);
    }

  private:
    Path m_sourcePath;
    Color m_baseColor;
    float m_metallic;
    float m_roughness;
    float m_ior;
    SharedPtr<ITexture> m_albedoTexture;
    SharedPtr<ITexture> m_normalTexture;
    SharedPtr<ITexture> m_ormTexture;
    SharedPtr<IShaderProgram> m_deferredGeometryShaderProgram;
    SharedPtr<IShaderProgram> m_forwardShaderProgram;

    void assertIsValid() const;
    void bindForDeferredGeometryPass(IDataBlockManager& dataBlockManager);
    void bindForForwardPass(IDataBlockManager& dataBlockManager);
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcShadingType.h"
#include "hc/graphics/resource/material/hcMaterialRenderMode.h"
#include "hc/graphics/resource/hcIGraphicResource.h"
#include "hc/graphics/hcRenderPassType.h"

namespace hc
{
  struct CameraMatrices;
  class AMaterialDescriptor;

  /**
   * @brief Interface for material objects in the engine.
   */
  class HC_CORE_EXPORT IMaterial : public IGraphicResource
  {
  public:
    virtual ~IMaterial() = default;

    /**
     * @brief Gets the unique identifier of the material.
     * 
     * @return Reference to the material's Id.
     */
    virtual UInt16 getMaterialId() const = 0;

    /**
     * @brief Gets the shader type associated with this material.
     * 
     * @return The shader type.
     */
    virtual shadingType::Type getShaderType() const = 0;

    /**
     * @brief Gets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     * 
     * @return The material render mode.
     */
    virtual materialRenderMode::Type getRenderMode() const = 0;

    /**
     * @brief Sets the render mode of the material, which determines how it should be
     * rendered (e.g., opaque, transparent).
     *
     * @param renderMode The material render mode to set.
     */
    virtual void setRenderMode(materialRenderMode::Type renderMode) = 0;

    /**
     * @brief Gets the alpha cutout threshold for the material. This value determines the
     * cutoff point for alpha testing when the material is rendered in alpha cutout mode.
     *
     * @return The alpha cutout threshold.
     */
    virtual float getAlphaCutoutThreshold() const = 0;

    /**
     * @brief Sets the alpha cutout threshold for the material. This value determines
     * the cutoff point for alpha testing when the material is rendered in alpha cutout
     * mode.
     *
     * @param threshold The alpha cutout threshold to set (range [0.0, 1.0]).
     */
    virtual void setAlphaCutoutThreshold(float threshold) = 0;

    /**
     * @brief Determines if the material is double-sided based on its render mode.
     *
     * @return True if the material is double-sided, false otherwise.
     */
    virtual bool isDoubleSided() const = 0;

    /**
     * @brief Sets whether the material should be rendered as double-sided, which
     * means that back-face culling will be disabled and both sides of the geometry
     * will be rendered.
     *
     * @param doubleSided True to make the material double-sided, false for single-sided.
     */
    virtual void setDoubleSided(bool doubleSided) = 0;

    /**
     * @brief Binds the material for rendering.
     *
     * @param cameraMatrices The camera matrices to use for rendering.
     * @param renderPass The render pass type.
     */
    virtual void bind(
      const CameraMatrices& cameraMatrices,
      renderPassType::Type renderPass
    ) = 0;

    /**
     * @brief Updates the model matrix uniform in the shader.
     * 
     * @param modelMatrix The model matrix to set.
     */
    virtual void updateModelMatrix(const Matrix4& modelMatrix) = 0;

    /**
     * @brief Unbinds the material after rendering.
     */
    virtual void unbind() = 0;

    /**
     * @brief Checks if the material is valid and ready for rendering.
     *
     * @return True if the material is valid, false otherwise.
     */
    virtual bool isValid() const = 0;

  protected:
    IMaterial() = default;
  };
}

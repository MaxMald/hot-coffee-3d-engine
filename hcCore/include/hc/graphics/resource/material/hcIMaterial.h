#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/material/hcShadingType.h"
#include "hc/graphics/resource/hcIGraphicResource.h"

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
     * @brief Binds the material for rendering.
     */
    virtual void bind(const CameraMatrices& cameraMatrices) = 0;

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
      * @brief Gets the material descriptor associated with this material.
      * 
      * @return Shared pointer to the material descriptor.
     */
    virtual SharedPtr<AMaterialDescriptor> getDescriptor() const = 0;

  protected:
    IMaterial() = default;
  };
}

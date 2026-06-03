#pragma once

#include "hc/graphics/resource/hcIGraphicResource.h"

namespace hc
{
  class IShader;

  /**
   * @brief Interface for a linked shader program (multi-stage).
   */
  class IShaderProgram : public IGraphicResource
  {
  public:
    virtual ~IShaderProgram() = default;

    /**
     * @brief Initializes the shader program with a set of shaders.
     */
    virtual void initialize(const Vector<SharedPtr<IShader>>& shaders) = 0;

    /**
     * @brief Binds this program for rendering.
     */
    virtual void bind() = 0;

    /**
     * @brief Returns true if the program is initialized and ready for use.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Sets a float uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, float value) = 0;

    /**
     * @brief Sets an integer uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, Int32 value) = 0;

    /**
     * @brief Sets a boolean uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, bool value) = 0;

    /**
     * @brief Sets a 3D vector uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, const Vector3f& v3f) = 0;

    /**
     * @brief Sets a color uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, const Color& color) = 0;

    /**
     * @brief Sets a 4x4 matrix uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniform(const String& name, const Matrix4& matrix4x4) = 0;

    /**
     * @brief Sets a texture uniform variable.
     *
     * @note Should only be called after the shader program is successfully linked and
     * bound.
     */
    virtual void setUniformTexture(const String& name, Int32 slotLocation) = 0;
  };
}

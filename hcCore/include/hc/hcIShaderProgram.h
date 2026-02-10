#pragma once

#include "hc/hcIGraphicResource.h"

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
     * @brief Binds this program for rendering.
     */
    virtual void bind() = 0;

    /**
     * @brief Links the attached shaders into a complete shader program.
     */
    virtual void linkShaders() = 0;

    /**
     * @brief Returns true if the program is successfully linked and ready.
     */
    virtual bool isValid() const = 0;

    /**
     * @brief Attaches a shader to this program.
     */
    virtual void attachShader(SharedPtr<IShader> shader) = 0;

    /**
     * @brief Sets a float uniform variable.
     */
    virtual void setUniform(const String& name, float value) = 0;

    /**
     * @brief Sets an integer uniform variable.
     */
    virtual void setUniform(const String& name, Int32 value) = 0;

    /**
     * @brief Sets a boolean uniform variable.
     */
    virtual void setUniform(const String& name, bool value) = 0;

    /**
     * @brief Sets a 3D vector uniform variable.
     */
    virtual void setUniform(const String& name, const Vector3f& v3f) = 0;

    /**
     * @brief Sets a color uniform variable.
     */
    virtual void setUniform(const String& name, const Color& color) = 0;

    /**
     * @brief Sets a 4x4 matrix uniform variable.
     */
    virtual void setUniform(const String& name, const Matrix4& matrix4x4) = 0;

    /**
     * @brief Sets a texture uniform variable.
     */
    virtual void setUniformTexture(const String& name, Int32 slotLocation) = 0;
  };
}

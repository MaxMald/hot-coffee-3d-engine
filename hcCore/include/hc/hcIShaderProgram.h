#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IShader;

  /**
   * @brief Interface for a linked shader program (multi-stage).
   */
  class IShaderProgram : public NonCopyable
  {
  public:
    virtual ~IShaderProgram() = default;

    /**
     * @brief Binds this program for rendering.
     */
    virtual void bind() = 0;

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
     * @brief Sets a 4x4 matrix uniform variable.
     */
    virtual void setUniform(const String& name, const float* matrix4x4) = 0;

    /**
     * @brief Destroys the shader program and releases resources.
     */
    virtual void destroy() = 0;
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/resource/texture/hcITexture.h"
#include "hc/graphics/resource/shaderProgram/hcIShaderProgram.h"

namespace hc
{
  namespace coreAssertions
  {
    /**
     * @brief Asserts that the given texture is valid and loaded. If the texture is null or
     * invalid, an InvalidArgumentException is thrown with a message indicating which texture is
     * invalid.
     *
     * @param texture The shared pointer to the texture to validate.
     * @param textureName The name of the texture (e.g., "Albedo", "Normal", "Specular") used in the error message.
     */
    inline void AssertTextureIsValid(
      const SharedPtr<ITexture>& texture,
      const String& textureName
    )
    {
      if (!texture || !texture->isValid())
        throw InvalidArgumentException(
          String::Format("%s texture is invalid.", textureName.c_str())
        );
    }

    /**
     * @brief Asserts that the given shader program is valid and linked. If the shader program is null
     * or invalid, an InvalidArgumentException is thrown with a message indicating that the shader program is invalid.
     *
     * @param shaderProgram The shared pointer to the shader program to validate.
     * @param shaderProgramName The name of the shader program used in the error message (optional).
     */
    inline void AssertShaderProgramIsValid(
      const SharedPtr<IShaderProgram>& shaderProgram,
      const String& shaderProgramName = "Shader program"
    )
    {
      if (!shaderProgram || !shaderProgram->isValid())
        throw InvalidArgumentException(
          String::Format("%s is invalid.", shaderProgramName.c_str())
        );
    }
  }
}

#pragma once

#include <shaderc/shaderc.hpp>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief A class responsible for compiling GLSL shaders to SPIR-V and creating OpenGL
   * shader objects.
   */
  class ShaderCompiler
  {
  public:
    ShaderCompiler();
    ~ShaderCompiler();

    /**
     * @brief Initializes the shader compiler with default options and a custom includer.
     */
    void initialize();

    /**
     * @brief Compiles GLSL shader source code to SPIR-V and creates an OpenGL shader
     * object.
     *
     * @param source The GLSL shader source code as a string.
     * @param type The type of shader stage (e.g., vertex, fragment).
     * @param sourceName The name of the source file (used for error reporting).
     * @param entryPoint The entry point function name in the shader code.
     *
     * @returns A shared pointer to the compiled IShader object.
     *
     * @throws RuntimeErrorException If the compilation fails or the shader cannot be
     * created.
     */
    SharedPtr<IShader> compileShaderFromString(
      const String& source,
      shaderStageType::Type type,
      const String& sourceName,
      const String& entryPoint
    );

  private:
    shaderc::CompileOptions m_compileOptions;
    shaderc::Compiler m_compiler;
  };
}

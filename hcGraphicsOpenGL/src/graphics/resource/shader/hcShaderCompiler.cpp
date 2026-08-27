#include "hc/graphics/resource/shader/hcShaderCompiler.h"
#include "hc/graphics/resource/shader/hcShaderIncluder.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"

namespace hc
{
  /**
   * @brief Converts a shader stage type to the corresponding shaderc_shader_kind.
   *
   * @param type The shader stage type to convert.
   *
   * @returns The corresponding shaderc_shader_kind.
   *
   * @throws RuntimeErrorException If the shader stage type is unsupported.
   */
  static shaderc_shader_kind ToShadercShaderKind(shaderStageType::Type type)
  {
    switch (type)
    {
    case shaderStageType::Vertex:
      return shaderc_vertex_shader;
    case shaderStageType::Fragment:
      return shaderc_fragment_shader;
    default:
      throw RuntimeErrorException(
        String::Format("Unsupported shader stage type: %d", static_cast<int>(type))
      );
    }
  }

  ShaderCompiler::ShaderCompiler() :
    m_compileOptions(),
    m_compiler()
  {}

  ShaderCompiler::~ShaderCompiler()
  {}

  void ShaderCompiler::initialize()
  {
    UniquePtr<ShaderIncluder> includer = MakeUnique<ShaderIncluder>();
    m_compileOptions.SetIncluder(std::move(includer));
  }

  SharedPtr<IShader> ShaderCompiler::compileShaderFromString(
    const String& source,
    shaderStageType::Type type,
    const String& sourceName,
    const String& entryPoint
  )
  {
    shaderc_shader_kind shaderKind = ToShadercShaderKind(type);
    shaderc::CompilationResult result = m_compiler.CompileGlslToSpv(
      source.c_str(),
      source.size(),
      shaderKind,
      sourceName.c_str(),
      m_compileOptions
    );

    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
      throw RuntimeErrorException(
        String::Format("Shader compilation failed: %s", result.GetErrorMessage().c_str())
      );
    }

    std::vector<UInt32> spirvCode(result.cbegin(), result.cend());

    SharedPtr<OpenGlShader> shader = MakeShared<OpenGlShader>();
    shader->initializeFromSpirv(type, spirvCode, entryPoint);
    return shader;
  }
}

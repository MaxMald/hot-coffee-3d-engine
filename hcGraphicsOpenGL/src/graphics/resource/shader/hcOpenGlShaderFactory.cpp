#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace hc
{
  /**
   * @brief Loads shader source code from a file located in the "shaders/opengl"
   * directory.
   *
   * @param relPath The relative path to the shader file within the "shaders/opengl"
   * directory.
   *
   * @returns The shader source code as a string.
   *
   * @throws RuntimeErrorException If the shader file cannot be opened.
   */
  static String LoadShaderSourceFromFile(const String& relPath)
  {
    std::filesystem::path path = std::filesystem::current_path() / "shaders/opengl" / relPath.c_str();
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
      throw new RuntimeErrorException(
        String::Format("Failed to open shader file: %s", path.string().c_str())
      );

    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

    return contents.str();
  }

  static String GetShaderFileNameFromBuiltInShaderType(builtInShaderType::Type type)
  {
    String strType = builtInShaderType::ToString(type);
    return String::Format("%s.glsl", strType.c_str());
  }

  SharedPtr<IShader> OpenGlShaderFactory::createShaderFromStringContent(
    shaderStageType::Type type,
    const String& content
  )
  {
    SharedPtr<OpenGlShader> shader = MakeShared<OpenGlShader>();
    shader->initialize(type, content);

    if (!shader->isValid())
    {
      shader.reset();
      throw RuntimeErrorException("Failed to create shader from string content.");
    }

    return shader;
  }

  SharedPtr<IShader> OpenGlShaderFactory::createBuiltInShaderType(builtInShaderType::Type type)
  {
    shaderStageType::Type stageType = builtInShaderType::GetShaderStageType(type);
    return createShaderFromStringContent(
      stageType,
      LoadShaderSourceFromFile(
        GetShaderFileNameFromBuiltInShaderType(type)
      )
    );
  }
}

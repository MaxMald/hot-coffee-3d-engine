#include "hc/graphics/resource/shader/hcOpenGlShaderManager.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace hc
{
  /**
   * @brief Resolves the absolute path of a shader file located in the "shaders/opengl"
   * directory.
   *
   * @param relPath The relative path to the shader file within the "shaders/opengl"
   * directory.
   *
   * @returns The absolute path to the shader file.
   */
  static Path ResolveShaderFilePath(const Path& relPath)
  {
    return std::filesystem::current_path() / "shaders/opengl" / relPath;
  }

  /**
   * @brief Loads shader source code from a file.
   *
   * @param path The absolute path to the shader file.
   *
   * @returns The shader source code as a string.
   *
   * @throws RuntimeErrorException If the shader file cannot be opened.
   */
  static String LoadShaderSourceFromFile(const Path& path)
  {
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

  /**
   * @brief Generates the filename for a built-in shader based on its type.
   *
   * @param type The type of the built-in shader.
   *
   * @returns The filename of the built-in shader, formatted as "<shader_type>.glsl".
   */
  static Path GetShaderFileNameFromBuiltInShaderType(builtInShaderType::Type type)
  {
    String strType = builtInShaderType::ToString(type);
    String pathStr = String::Format("%s.glsl", strType.c_str());
    return Path(pathStr.c_str());
  }

  OpenGlShaderManager::OpenGlShaderManager() :
    AShaderManager()
  {}

  OpenGlShaderManager::~OpenGlShaderManager()
  {}

  void OpenGlShaderManager::initialize()
  {
    m_shaderCompiler.initialize();
  }

  SharedPtr<IShader> OpenGlShaderManager::createConcreteShaderFromString(
    const String& content,
    shaderStageType::Type type
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

  SharedPtr<IShader> OpenGlShaderManager::createConcreteBuiltInShader(
    builtInShaderType::Type type
  )
  {
    try
    {
      shaderStageType::Type stageType = builtInShaderType::GetShaderStageType(type);
      Path shaderFilePath = ResolveShaderFilePath(
        GetShaderFileNameFromBuiltInShaderType(type)
      );

      SharedPtr<IShader> shader = compileSpirvAndCreateShaderFromFile(
        shaderFilePath,
        stageType,
        "main"
      );

      return shader;
    }
    catch (const Exception & e)
    {
      throw RuntimeErrorException(
        String::Format(
          "Failed to create built-in shader of type %s: %s",
          builtInShaderType::ToString(type).c_str(),
          e.what()
        )
      );
    }
  }

  void OpenGlShaderManager::onClear()
  {
    // TODO
  }

  void OpenGlShaderManager::onDestroy()
  {
    // TODO
  }

  SharedPtr<IShader> OpenGlShaderManager::compileSpirvAndCreateShaderFromFile(
    const Path& filePath,
    shaderStageType::Type stageType,
    const String& entryPoint
  )
  {
    try
    {
      String source = LoadShaderSourceFromFile(filePath);
      SharedPtr<IShader> shader = m_shaderCompiler.compileShaderFromString(
        source,
        stageType,
        filePath.string(),
        entryPoint
      );
      return shader;
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        String::Format(
          "Failed to compile shader to SPIR-V from file %s: %s",
          filePath.string().c_str(),
          e.what()
        )
      );
    }
  }
}

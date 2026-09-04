#pragma once

#include <hc/graphics/resource/shader/hcAShaderManager.h>

#include "hc/hcGraphicsOpenGlPrerequisites.h"
#include "hc/graphics/resource/shader/hcShaderCompiler.h"

namespace hc
{
  class OpenGlShaderManager : public AShaderManager
  {
  public:
    OpenGlShaderManager();
    virtual ~OpenGlShaderManager() override;

    /**
     * @copydoc IShaderManager::initialize
     */
    virtual void initialize() override;

  protected:
    ShaderCompiler m_shaderCompiler;

    virtual SharedPtr<IShader> createConcreteShaderFromString(
      const String& content,
      shaderStageType::Type type
    ) override;

    virtual SharedPtr<IShader> createConcreteBuiltInShader(
      builtInShaderType::Type type
    ) override;

    virtual void onClear() override;

    virtual void onDestroy() override;

  private:

    /**
     * @brief Compiles a shader using shaderc from a file and creates an OpenGL shader
     * object.
     *
     * @param filePath The path to the shader source file.
     * @param stageType The type of shader stage (e.g., vertex, fragment).
     * @param entryPoint The entry point function name in the shader source.
     *
     * @returns A shared pointer to the created IShader object.
     */
    SharedPtr<IShader> compileSpirvAndCreateShaderFromFile(
      const Path& filePath,
      shaderStageType::Type stageType,
      const String& entryPoint
    );
  };
}

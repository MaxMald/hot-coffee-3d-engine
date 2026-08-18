#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shader/hcBuiltInShaders.h"
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
   */
  static String LoadShaderSourceFromFile(const String& relPath)
  {
    std::filesystem::path path = std::filesystem::current_path() / "shaders/opengl" / relPath.c_str();
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
      return String();

    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

    return contents.str();
  }

  static String GetShaderFileNameFromBuiltInShaderType(builtInShaderType::Type type)
  {
    switch (type)
    {
    case builtInShaderType::UnlitVertex:
      return "vs-unlit.glsl";
    case builtInShaderType::UnlitFragment:
      return "fs-unlit.glsl";
    case builtInShaderType::LitVertex:
      return "vs-lit.glsl";
    case builtInShaderType::BlinnPhongForwardFragment:
      return "fs-blinn-phong-forward.glsl";
    case builtInShaderType::BlinnPhongDeferredFragment:
      return "fs-blinn-phong-deferred.glsl";
    case builtInShaderType::FullScreenTriangleVertex:
      return "vs-full-screen-triangle.glsl";
    case builtInShaderType::DeferredLightingFragment:
      return "fs-deferred-lighting.glsl";
    case builtInShaderType::SkyboxVertex:
      return "vs-skybox.glsl";
    case builtInShaderType::SkyboxFragment:
      return "fs-skybox.glsl";
    case builtInShaderType::FinalPassFragment:
      return "fs-final-pass.glsl";
    case builtInShaderType::ShadowMapVertex:
      return "vs-shadow-map.glsl";
    case builtInShaderType::ShadowMapFragment:
      return "fs-shadow-map.glsl";
    default:
      throw RuntimeErrorException(
        String::Format(
          "Built-in shader type '%u' is not implemented in OpenGlShaderFactory.",
          static_cast<UInt32>(type)
        )
      );
    }
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
    switch (type)
    {
    case builtInShaderType::UnlitVertex:
      return createShaderFromStringContent(
        shaderStageType::Vertex,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::UnlitFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::LitVertex:
      return createShaderFromStringContent(
        shaderStageType::Vertex,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::BlinnPhongForwardFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::BlinnPhongDeferredFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::FullScreenTriangleVertex:
      return createShaderFromStringContent(
        shaderStageType::Vertex,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::DeferredLightingFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::SkyboxVertex:
      return createShaderFromStringContent(
        shaderStageType::Vertex,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::SkyboxFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::FinalPassFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::ShadowMapVertex:
      return createShaderFromStringContent(
        shaderStageType::Vertex,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    case builtInShaderType::ShadowMapFragment:
      return createShaderFromStringContent(
        shaderStageType::Fragment,
        LoadShaderSourceFromFile(
          GetShaderFileNameFromBuiltInShaderType(type)
        )
      );
    default:
      throw RuntimeErrorException(
        String::Format(
          "Built-in shader type '%u' is not implemented in OpenGlShaderFactory.",
          static_cast<UInt32>(type)
        )
      );
    }
  }
}

#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shader/hcBuiltInShaders.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"

namespace hc
{
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
      return createShaderFromStringContent(shaderStageType::Vertex, builtInShaders::UnlitVertex);
    case builtInShaderType::UnlitFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::UnlitFragment);
    case builtInShaderType::LitVertex:
      return createShaderFromStringContent(shaderStageType::Vertex, builtInShaders::LitVertex);
    case builtInShaderType::BlinnPhongForwardFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::BlinnPhongForwardFragment);
    case builtInShaderType::BlinnPhongDeferredFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::BlinnPhongDeferredFragment);
    case builtInShaderType::FullScreenTriangleVertex:
      return createShaderFromStringContent(shaderStageType::Vertex, builtInShaders::FullScreenTriangleVertex);
    case builtInShaderType::DeferredLightingFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::DeferredLightingFragment);
    case builtInShaderType::SkyboxVertex:
      return createShaderFromStringContent(shaderStageType::Vertex, builtInShaders::SkyboxVertex);
    case builtInShaderType::SkyboxFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::SkyboxFragment);
    case builtInShaderType::FinalPassFragment:
      return createShaderFromStringContent(shaderStageType::Fragment, builtInShaders::FinalPassFragment);
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

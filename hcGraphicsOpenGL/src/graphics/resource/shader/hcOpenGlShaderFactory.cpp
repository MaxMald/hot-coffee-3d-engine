#include "hc/graphics/resource/shader/hcOpenGlShaderFactory.h"
#include "hc/graphics/resource/shader/hcBuiltInShaders.h"
#include "hc/graphics/resource/shader/hcOpenGlShader.h"

namespace hc
{
  SharedPtr<IShader> OpenGlShaderFactory::createShaderFromStringContent(
    const String& content,
    shaderStageType::Type type
  )
  {
    return MakeShared<OpenGlShader>(type, content);
  }

  SharedPtr<IShader> OpenGlShaderFactory::createBuiltInShaderType(builtInShaderType::Type type)
  {
    switch (type)
    {
    case builtInShaderType::UnlitVertex:
      return MakeShared<OpenGlShader>(shaderStageType::Vertex, builtInShaders::UnlitVertex);
    case builtInShaderType::UnlitFragment:
      return MakeShared<OpenGlShader>(shaderStageType::Fragment, builtInShaders::UnlitFragment);
    case builtInShaderType::LitVertex:
      return MakeShared<OpenGlShader>(shaderStageType::Vertex, builtInShaders::LitVertex);
    case builtInShaderType::BlinnPhongForwardFragment:
      return MakeShared<OpenGlShader>(shaderStageType::Fragment, builtInShaders::BlinnPhongForwardFragment);
    case builtInShaderType::BlinnPhongDeferredFragment:
      return MakeShared<OpenGlShader>(shaderStageType::Fragment, builtInShaders::BlinnPhongDeferredFragment);
    case builtInShaderType::FullScreenTriangleVertex:
      return MakeShared<OpenGlShader>(shaderStageType::Vertex, builtInShaders::FullScreenTriangleVertex);
    case builtInShaderType::DeferredLightingFragment:
      return MakeShared<OpenGlShader>(shaderStageType::Fragment, builtInShaders::DeferredLightingFragment);
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

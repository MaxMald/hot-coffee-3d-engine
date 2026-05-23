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

  SharedPtr<IShader> OpenGlShaderFactory::createDefaultVertexShader()
  {
    return MakeShared<OpenGlShader>(
      shaderStageType::Vertex,
      builtInShaders::VertexShader
    );
  }

  SharedPtr<IShader> OpenGlShaderFactory::createLitVertexShader()
  {
    return MakeShared<OpenGlShader>(
      shaderStageType::Vertex,
      builtInShaders::LitVertexShader
    );
  }

  SharedPtr<IShader> OpenGlShaderFactory::createUnlitFragmentShader()
  {
    return MakeShared<OpenGlShader>(
      shaderStageType::Fragment,
      builtInShaders::UnlitFragment
    );
  }

  SharedPtr<IShader> OpenGlShaderFactory::createBlinnPhongForwardFragmentShader()
  {
    return MakeShared<OpenGlShader>(
      shaderStageType::Fragment,
      builtInShaders::BlinnPhongForwardFragment
    );
  }

  SharedPtr<IShader> OpenGlShaderFactory::createBlinnPhongDeferredFragmentShader()
  {
    // TODO
    // Create a blinn-phon deferred
    throw RuntimeErrorException("Not Implemeted yet.");
  }
}

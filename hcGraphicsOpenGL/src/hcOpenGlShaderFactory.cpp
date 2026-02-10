#include "hc/hcOpenGlShaderFactory.h"
#include "hc/hcBuiltInShaders.h"
#include "hc/hcOpenGlShader.h"

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

  SharedPtr<IShader> OpenGlShaderFactory::createUnlitFragmentShader()
  {
    return MakeShared<OpenGlShader>(
      shaderStageType::Fragment,
      builtInShaders::UnlitFragment
    );
  }
}

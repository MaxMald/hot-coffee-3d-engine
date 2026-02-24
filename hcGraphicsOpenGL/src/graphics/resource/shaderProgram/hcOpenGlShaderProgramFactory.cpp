#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgramFactory.h"
#include "hc/graphics/resource/shaderProgram/hcOpenGlShaderProgram.h"

namespace hc
{
  SharedPtr<IShaderProgram> OpenGlShaderProgramFactory::createShaderProgram()
  {
    return MakeShared<OpenGlShaderProgram>();
  }
}

#include "hc/hcOpenGlShaderProgramFactory.h"
#include "hc/hcOpenGlShaderProgram.h"

namespace hc
{
  SharedPtr<IShaderProgram> OpenGlShaderProgramFactory::createShaderProgram()
  {
    return MakeShared<OpenGlShaderProgram>();
  }
}

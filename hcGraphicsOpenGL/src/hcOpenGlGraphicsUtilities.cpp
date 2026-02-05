#include "hc/hcOpenGlGraphicsUtilitites.h"
#include <GL/glew.h>

namespace hc
{
  namespace openGlGraphicsUtilities
  {
    void checkAndLogPosibleError()
    {
      GLenum error = glGetError();
      if (error != GL_NO_ERROR)
      {
        LogService::Error(
          String("OpenGL error: ") +
          String(reinterpret_cast<const char*>(glewGetErrorString(error)))
        );
      }
    }
  }
}

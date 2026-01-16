#include "hc/hcOpenGlGraphicsManager.h"
#include <GL/glew.h>
#include "hc/hcDependencyContainer.h"
#include "hc/hcIWindow.h"

namespace hc
{
  OpenGlGraphicsManager::OpenGlGraphicsManager()
  {
  }

  OpenGlGraphicsManager::~OpenGlGraphicsManager()
  {
  }

  void OpenGlGraphicsManager::init()
  {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
      throw RuntimeErrorException(
        "Failed to initialize GLEW: " +
        String(reinterpret_cast<const char*>(glewGetErrorString(err)))
      );
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  }

  void OpenGlGraphicsManager::draw(IWindow& window)
  {
    // Begin frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw stuff

    // End frame
    window.swapBuffers();
  }

  void OpenGlGraphicsManager::destroy()
  {
  }
}

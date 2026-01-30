#include "hc/hcOpenGlGraphicsManager.h"
#include <GL/glew.h>
#include "hc/hcDependencyContainer.h"
#include "hc/hcIWindow.h"
#include "hc/hcIDrawable.h"

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

    // Set up basic OpenGL state
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
  }

  void OpenGlGraphicsManager::draw(IDrawable& drawable)
  {
    drawable.draw(*this);
  }

  void OpenGlGraphicsManager::beginFrame()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGlGraphicsManager::endFrame(IWindow& window)
  {
    window.swapBuffers();
  }

  SharedPtr<ITexture> OpenGlGraphicsManager::createTexture(SharedPtr<Image> image) const
  {
    // TODO Texture creation
    return nullptr;
  }

  SharedPtr<IShader> OpenGlGraphicsManager::createShaderFromString(
    const String& shaderCode
  ) const
  {
    // TODO Shader creation
    return SharedPtr<IShader>();
  }

  void OpenGlGraphicsManager::destroy()
  {
  }
}

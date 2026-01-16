#include "hc/hcOpenGlGraphicsPlugin.h"

#include "hc/hcDependencyContainer.h"
#include "hc/hcOpenGlGraphicsManager.h"

namespace hc
{
  HC_GRAPHICS_OPENGL_EXPORT IPlugin* createGraphicsOpenGLPlugin()
  {
    return new OpenGlGraphicsPlugin();
  }

  HC_GRAPHICS_OPENGL_EXPORT void destroyGraphicsOpenGLPlugin()
  {
  }

  OpenGlGraphicsPlugin::OpenGlGraphicsPlugin()
  {
  }

  void OpenGlGraphicsPlugin::onConnect()
  {
  }

  void OpenGlGraphicsPlugin::onClose()
  {
  }

  void* OpenGlGraphicsPlugin::getData()
  {
    return nullptr;
  }

  void OpenGlGraphicsPlugin::addDependencies(DependencyContainer& container)
  {
    container.registerInstanceAsInterface<IGraphicsManager, OpenGlGraphicsManager>(
      MakeShared<OpenGlGraphicsManager>()
    );
  }
}

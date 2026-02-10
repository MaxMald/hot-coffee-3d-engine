#include "hc/hcOpenGlGraphicsPlugin.h"

#include <hc/hcMaterialFactoriesManager.h>
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

  void OpenGlGraphicsPlugin::addDependencies(DependencyContainer& container)
  {
  }

  UniquePtr<IGraphicsManager> OpenGlGraphicsPlugin::createGraphicsManager(
    IWindow& window,
    AssetManager& assetManager,
    UniquePtr<MaterialFactoriesManager> materialFactoriesManager
  ) const
  {
    return MakeUnique<OpenGlGraphicsManager>(
      window,
      assetManager,
      std::move(materialFactoriesManager)
    );
  }
}

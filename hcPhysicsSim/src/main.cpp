#include <hc/hcHotCoffeeCore.h>
#include <iostream>

#include "game.h"

int main()
{
  hc::LogService::Prepare();

  hc::HotCoffeeEngineSettings settings;
  settings.windowSettings.width = 1920;
  settings.windowSettings.height = 1080;
  settings.windowSettings.title = "Physics Simulation";
  settings.pluginManagerSettings.windowType =
    hc::PluginManagerSettings::windowPluginType::SFML;
  settings.pluginManagerSettings.graphicsType =
    hc::PluginManagerSettings::graphicsPluginType::OPENGL;

  hc::HotCoffeeEngine engine;
  hc::ProcessResult proccResult = engine.initialize(settings);
  if (!proccResult.success)
    return 0;

  hc::Path currentPath = hc::Path::GetCurrentPath();
  currentPath /= hc::Path("../../../hcPhysicsSim/assets");
  engine.getAssetManager().setRootPath(currentPath);

  // Scene creation
  engine.getSceneManager().createScene<Game>("PhysicsSimulationScene", &engine);
  engine.getSceneManager().setActiveScene("PhysicsSimulationScene");

  try
  {
    engine.run();
  }
  catch (const hc::Exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }

  engine.destroy();
  return 0;
}

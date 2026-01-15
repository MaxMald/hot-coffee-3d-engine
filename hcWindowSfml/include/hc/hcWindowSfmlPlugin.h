#pragma once

#include "hc/hcWindowSfmlPrerequisites.h"
#include "hc/hcIPlugin.h"

namespace hc
{
  extern "C"
  {
    HC_WINDOW_SFML_EXPORT IPlugin* createWindowSfmlPlugin();
    HC_WINDOW_SFML_EXPORT void destroyWindowSfmlPlugin();
  }

  class HC_WINDOW_SFML_EXPORT WindowSfmlPlugin : public IPlugin
  {
  public:
    WindowSfmlPlugin();
    virtual ~WindowSfmlPlugin();

    void onConnect() override;
    void onClose() override;
    void* getData() override;
    void addDependencies(DependencyContainer& container) override;
  };
}

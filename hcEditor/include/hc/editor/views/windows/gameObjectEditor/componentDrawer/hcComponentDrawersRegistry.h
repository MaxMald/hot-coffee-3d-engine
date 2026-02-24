#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class ComponentDrawersManager;
  class ProjectFileSelectorView;

  namespace componentDrawersRegistry
  {
    /**
     * @brief Registers the default component drawers into the provided manager.
     */
    void registryDefaultComponentDrawers(
      HotCoffeeEngine& hotCoffeeEngine,
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileSelectorView& projectFileSelector
    );
  }
}

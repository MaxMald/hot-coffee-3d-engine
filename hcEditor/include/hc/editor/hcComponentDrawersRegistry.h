#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class ComponentDrawersManager;
  class ProjectFileSelector;

  namespace componentDrawersRegistry
  {
    /**
     * @brief Registers the default component drawers into the provided manager.
     */
    void registryDefaultComponentDrawers(
      ComponentDrawersManager& componentDrawersManager,
      ProjectFileSelector& projectFileSelector
    );
  }
}

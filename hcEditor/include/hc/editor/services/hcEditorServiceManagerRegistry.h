#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class EditorServiceManager;

  namespace editorServiceManagerRegistry
  {
    void registerServices(
      HotCoffeeEngine& engine,
      EditorServiceManager& serviceManager,
      Scene* editorScene
    );
  }
}

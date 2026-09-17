#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  namespace comboItems
  {
    static const char* RENDER_MODE[] = {
      "Background",
      "Opaque",
      "AlphaCutout",
      "Transparent"
    };
    static Int32 RENDER_MODE_COUNT = sizeof(RENDER_MODE) / sizeof(RENDER_MODE[0]);
  }

  namespace style
  {
    static float COMPONENT_MAT_TEXTURE_SIZE = 64.0f;
  }
}

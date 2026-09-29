#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/style/hcStyle.h"

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
}

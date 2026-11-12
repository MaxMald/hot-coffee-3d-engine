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

    static const char* LIGHT_TYPE[] = {
      "Directional",
      "Omni",
      "Spot"
    };
    static Int32 LIGHT_TYPE_COUNT = sizeof(LIGHT_TYPE) / sizeof(LIGHT_TYPE[0]);

    constexpr const hc::Char* COMPONENT_TYPES[] =
    {
      "Mesh",
      "Camera",
      "Light"
    };
    static Int32 COMPONENT_TYPES_COUNT = sizeof(COMPONENT_TYPES) / sizeof(COMPONENT_TYPES[0]);
  }
}

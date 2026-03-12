#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/hcDrawCommand.h"

namespace hc
{
  struct DrawCommandUtilities
  {
    /**
     * Sorts a list of draw commands based on their sort keys.
      *
      * Draw commands are sorted in ascending order of their sort keys, which
      * are generated based on render queue, material ID and distance to camera.
      *
      * @param drawCommands The vector of draw commands to be sorted.
     */
    static inline void SortDrawCommands(Vector<DrawCommand>& drawCommands)
    {
      std::sort(
        drawCommands.begin(),
        drawCommands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
          return a.sortKey < b.sortKey;
        }
      );
    }
  };
}

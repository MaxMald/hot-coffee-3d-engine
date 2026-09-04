#pragma once

#include <algorithm>
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

    /**
     * Splits draw commands into deferred and forward command lists for
     * hybrid rendering.
     *
     * Classification rules:
     * - Opaque and alpha-cutout commands go to the deferred list.
     * - Transparent commands go to the forward list.
     * - Unlit commands go to the forward list.
     *
     * @param drawCommands Source commands to classify.
     * @param deferredRenderingCommands Output list for deferred geometry pass.
     * @param forwardRenderingCommands Output list for forward passes.
     */
    static inline void SplitDrawCommandsByPipelinePath(
      const Vector<DrawCommand>& drawCommands,
      Vector<DrawCommand>& deferredRenderingCommands,
      Vector<DrawCommand>& forwardRenderingCommands
    )
    {
      for (SizeT i = 0; i < drawCommands.size(); ++i)
      {
        const DrawCommand& cmd = drawCommands[i];

        if (!cmd.material)
          continue;

        if (cmd.material->getMaterialType() == materialType::Unlit)
        {
          // Unlit materials are rendered in the forward rendering pass to ensure
          // correct rendering without lighting calculations.

          forwardRenderingCommands.push_back(cmd);
          continue;
        }

        if (cmd.material->getRenderMode() == materialRenderMode::Type::Opaque ||
          cmd.material->getRenderMode() == materialRenderMode::Type::AlphaCutout)
        {
          deferredRenderingCommands.push_back(cmd);
        }
        else
        {
          // Transparent objects are rendered in the forward rendering pass to ensure
          // correct blending and depth sorting.

          forwardRenderingCommands.push_back(cmd);
        }
      }
    }
  };
}

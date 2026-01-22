#pragma once

/****************************************************************************/
/* Note: All methods in this file use asset keys as paths for dependencies, */
/* as the editor always saves asset keys as their corresponding paths.      */
/****************************************************************************/

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class MaterialDescriptor;
}

namespace hc::editor
{
  namespace assetDependenciesLoader
  {
    void loadDependenciesForMaterialDescriptor(
      const Path& assetPath,
      SharedPtr<MaterialDescriptor>& materialDescriptor
    );
  }
}

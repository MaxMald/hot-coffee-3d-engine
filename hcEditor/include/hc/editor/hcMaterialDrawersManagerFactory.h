#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class MaterialDrawersManager;

  namespace materialDrawerManagerFactory
  {
    UniquePtr<MaterialDrawersManager> create();
  }
}

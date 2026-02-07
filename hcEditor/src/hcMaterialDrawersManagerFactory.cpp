#include "hc/editor/hcMaterialDrawersManagerFactory.h"
#include "hc/editor/hcMaterialDrawersManager.h"
#include "hc/editor/hcUnlitMaterialDrawer.h"

namespace hc::editor
{
  namespace materialDrawerManagerFactory
  {
    UniquePtr<MaterialDrawersManager> create()
    {
      UniquePtr<MaterialDrawersManager> manager
        = MakeUnique<MaterialDrawersManager>();

      manager->addDrawer(MakeUnique<UnlitMaterialDrawer>());

      return std::move(manager);
    }
  }
}

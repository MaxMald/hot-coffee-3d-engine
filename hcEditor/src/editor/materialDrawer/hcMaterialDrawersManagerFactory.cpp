#include "hc/editor/materialDrawer/hcMaterialDrawersManagerFactory.h"
#include "hc/editor/materialDrawer/hcMaterialDrawersManager.h"
#include "hc/editor/materialDrawer/hcUnlitMaterialDrawer.h"

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

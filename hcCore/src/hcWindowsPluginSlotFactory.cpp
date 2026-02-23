#include "hc/plugins/pluginSlot/hcWindowsPluginSlotFactory.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32

#include "hc/plugins/pluginSlot/hcWindowPluginSlot.h"

namespace hc
{
  SharedPtr<IPluginSlot> WindowsPluginSlotFactory::createPluginSlot()
  {
    return MakeShared<WindowsPluginSlot>();
  }
}

#endif // HC_PLATFORM == HC_PLATFORM_WIN32

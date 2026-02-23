#include "hc/plugins/pluginSlot/hcLinuxPluginSlotFactory.h"

#if HC_PLATFORM == HC_PLATFORM_LINUX

#include "hc/plugins/pluginSlot/hcLinuxPluginSlot.h"

namespace hc
{
  SharedPtr<IPluginSlot> LinuxPluginSlotFactory::createPluginSlot()
  {
    return MakeShared<LinuxPluginSlot>();
  }
}

#endif // HC_PLATFORM == HC_PLATFORM_LINUX

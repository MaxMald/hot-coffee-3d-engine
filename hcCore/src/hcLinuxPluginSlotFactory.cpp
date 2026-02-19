#if HC_PLATFORM == HC_PLATFORM_LINUX

#include "hc/hcLinuxPluginSlotFactory.h"
#include "hc/hcLinuxPluginSlot.h"

namespace hc
{
  SharedPtr<IPluginSlot> LinuxPluginSlotFactory::createPluginSlot()
  {
    return MakeShared<LinuxPluginSlot>();
  }
}

#endif // HC_PLATFORM == HC_PLATFORM_LINUX

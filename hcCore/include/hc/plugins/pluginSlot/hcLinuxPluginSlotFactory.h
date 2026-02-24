#pragma once

#include "hc/hcCorePrerequisites.h"

#if HC_PLATFORM == HC_PLATFORM_LINUX

#include "hc/plugins/pluginSlot/hcIPluginSlotFactory.h"

namespace hc
{
  /**
   * @brief Linux platform specific implementation of IPluginSlotFactory.
   */
  class LinuxPluginSlotFactory : public IPluginSlotFactory
  {
  public:
    SharedPtr<IPluginSlot> createPluginSlot() override;
  };
}

#endif // HC_PLATFORM == HC_PLATFORM_LINUX

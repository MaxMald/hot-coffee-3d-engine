#pragma once

#include "hc/hcCorePrerequisites.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32

#include "hc/hcIPluginSlotFactory.h"

namespace hc
{
  /**
   * @brief Windows platform specific implementation of IPluginSlotFactory.
   */
  class WindowsPluginSlotFactory : public IPluginSlotFactory
  {
  public:
    SharedPtr<IPluginSlot> createPluginSlot() override;
  };
}

#endif // HC_PLATFORM == HC_PLATFORM_WIN32

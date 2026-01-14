#pragma once

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

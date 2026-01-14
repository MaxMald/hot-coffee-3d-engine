#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IPluginSlot;

  /**
   * @brief Factory interface for creating IPluginSlot instances.
   */
  class IPluginSlotFactory
  {
  public:
    virtual ~IPluginSlotFactory() = default;

    /**
     * Creates a new instance of IPluginSlot.
     *
     * @return SharedPtr<IPluginSlot> A shared pointer to the created IPluginSlot
     * instance.
     */
    virtual SharedPtr<IPluginSlot> createPluginSlot() = 0;
  };
}

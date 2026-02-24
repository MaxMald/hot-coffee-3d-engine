#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class HotCoffeeEngine;
  class Event;

  /**
   * @brief Interface for objects that want to receive and handle events.
   *
   * Implement this interface to receive event notifications from the engine.
   */
  class HC_CORE_EXPORT IEventListener
  {
  public:
    virtual ~IEventListener() = default;

  protected:
    IEventListener() = default;

    /**
     * @brief Called when an event is dispatched to this listener.
     *
     * @param event The event object containing event data.
     *
     * @return True if the event was handled and should not be propagated
     * further, false to allow other listeners to process it.
     */
    virtual bool onEvent(const Event& event) = 0;

    friend class HotCoffeeEngine;
  };
}

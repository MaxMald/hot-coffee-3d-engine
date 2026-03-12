#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class HotCoffeeEngine;
  class Event;

  class HC_CORE_EXPORT IGameLoopListener
  {
  public:
    virtual ~IGameLoopListener() = default;

  protected:
    IGameLoopListener() = default;

    /**
     * @brief Called when an event is dispatched to this listener.
     *
     * @param event The event object containing event data.
     *
     * @return True if the event was handled and should not be propagated
     * further, false to allow other listeners to process it.
     */
    virtual bool onEvent(const Event& event) = 0;

    /**
     * @brief Called after the scene is rendered in each frame.
      *
      * Override this method to perform custom logic after the scene has been rendered.
     */
    virtual void onAfterSceneRender() = 0;

    friend class HotCoffeeEngine;
  };
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for editor services that require periodic updates.
   *
   * Classes implementing this interface must provide an implementation for the
   * update method, which is called with the elapsed time since the last update.
   */
  class IUpdatableEditorService
  {
  public:
    virtual ~IUpdatableEditorService() = default;

    /**
     * @brief Updates the service.
     *
     * This method should be called once per frame or tick, allowing the service
     * to perform time-dependent operations.
     *
     * @param elapsedTime The time elapsed since the last update.
     */
    virtual void update(const Time& elapsedTime) = 0;

  protected:
    IUpdatableEditorService() = default;
  };
}

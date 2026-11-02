#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for classes that want to listen to editor scene manager events.
   *
   * Implement this interface to receive notifications when scenes are opened, closed, or
   * cleared in the editor.
   */
  class IEditorSceneManagerListener
  {
  public:
    virtual ~IEditorSceneManagerListener() = default;

    /**
     * @brief Called when a scene is opened in the editor.
     */
    virtual void onSceneOpened() = 0;

    /**
     * @brief Called when a scene is closed in the editor.
     */
    virtual void onSceneClosed() = 0;

    /**
     * @brief Called when a scene is cleared in the editor.
     */
    virtual void onSceneCleared() = 0;

  protected:
    IEditorSceneManagerListener() = default;
  };
}

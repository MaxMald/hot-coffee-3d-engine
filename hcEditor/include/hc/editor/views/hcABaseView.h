#pragma once

#include "hc/editor/views/hcIView.h"

namespace hc::editor
{
  /**
   * @brief Abstract base class for editor views. Automatically registers and
   * unregisters the view with the EditorViewsManager.
   */
  class ABaseView : public IView
  {
  public:
    virtual ~ABaseView() override = default;

    /**
     * @brief Destroys the view and performs cleanup.
     *
     * Calls the onDestroy() method for custom cleanup logic in derived classes.
     */
    void destroy() override;

  protected:
    ABaseView();

    /**
     * @brief Custom destruction logic to be implemented by derived classes.
     *
     * Called by destroy() before the object is destroyed.
     */
    virtual void onDestroy();
  };
}

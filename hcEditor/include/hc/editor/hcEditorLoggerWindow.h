#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  /**
   * @brief Window view for displaying editor log messages.
   */
  class EditorLoggerWindow : public AWindowView
  {
  public:
    EditorLoggerWindow();
    virtual ~EditorLoggerWindow();

  protected:
    bool m_autoScroll;

    virtual void onDraw() override;
  };;
}

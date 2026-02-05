#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class EditorLogHistory;

  /**
   * @brief Window view for displaying editor log messages.
   */
  class EditorLoggerWindow : public AWindowView
  {
  public:
    EditorLoggerWindow(EditorLogHistory& logHistory);
    virtual ~EditorLoggerWindow();

  protected:
    EditorLogHistory& m_logHistory;
    bool m_autoScroll;

    virtual void onDraw() override;
  };;
}

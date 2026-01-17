#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class EditorLogger;

  class EditorLoggerWindow : public AWindowView
  {
  public:
    EditorLoggerWindow(EditorLogger&);
    virtual ~EditorLoggerWindow();

  protected:
    EditorLogger& m_editorLogger;
    bool m_autoScroll;

    virtual void onDraw() override;
  };;
}

#pragma once

#include "hc/editor/hcAWindowView.h"

namespace hc::editor
{
  class EditorLogger;

  /**
   * @brief Window view for displaying editor log messages.
   */
  class EditorLoggerWindow : public AWindowView, public IDependencyResolvable
  {
  public:
    EditorLoggerWindow();
    virtual ~EditorLoggerWindow();

    void resolveDependencies(DependencyContainer& container) override;

  protected:
    SharedPtr<EditorLogger> m_editorLogger;
    bool m_autoScroll;

    virtual void onDraw() override;
  };;
}

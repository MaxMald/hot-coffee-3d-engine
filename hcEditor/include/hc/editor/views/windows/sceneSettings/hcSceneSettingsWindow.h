#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  class EditorSceneManager;

  class SceneSettingsWindow : public AWindowView
  {
  public:
    SceneSettingsWindow(EditorSceneManager& editorSceneManager);
    ~SceneSettingsWindow() override;

    virtual void destroy() override;

  protected:
    virtual void onDraw() override;

  private:
    EditorSceneManager& m_editorSceneManager;
  };
}

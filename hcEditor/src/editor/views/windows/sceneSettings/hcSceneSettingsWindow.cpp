#include "hc/editor/views/windows/sceneSettings/hcSceneSettingsWindow.h"

#include <imgui.h>

#include "hc/editor/services/editorSceneManager/hcEditorSceneManager.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  SceneSettingsWindow::SceneSettingsWindow(EditorSceneManager& editorSceneManager) :
    AWindowView("Scene Settings", false, Vector2f(400.0f, 300.0f)),
    m_editorSceneManager(editorSceneManager)
  {
  }

  SceneSettingsWindow::~SceneSettingsWindow()
  {
  }

  void  SceneSettingsWindow::destroy()
  {
    // No specific resources to release in this implementation.
  }

  void SceneSettingsWindow::onDraw()
  {
    if (!m_editorSceneManager.isSceneOpen())
      return;

    Scene& scene = m_editorSceneManager.getEditorScene();
    SceneSettings& settings = scene.getSettings();

    imguiUtilities::DrawColorEdit3("Ambient Color", settings.ambientColor);
    ImGui::SliderFloat(
      "Ambient Intensity",
      &settings.ambientIntensity,
      0.0f,
      1.0f
    );
  }
}

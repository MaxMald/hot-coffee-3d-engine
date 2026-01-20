#include "hc/editor/hcLightManagerWindow.h"

#include <hc/hcDependencyContainer.h>
#include <hc/hcSceneManager.h>
#include <hc/hcScene.h>
#include <hc/hcLightManager.h>
#include <hc/hcLight.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "imgui.h"

namespace hc::editor
{
  LightManagerWindow::LightManagerWindow() :
    AWindowView("Light Manager", false)
  {
  }

  LightManagerWindow::~LightManagerWindow() = default;

  void LightManagerWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_sceneManager = container.resolve<SceneManager>();
  }

  void LightManagerWindow::onDraw()
  {
    if (!m_sceneManager)
      return;

    Scene* activeScene = m_sceneManager->getActiveScene();
    if (!activeScene)
    {
      ImGui::Text("No active scene.");
      return;
    }

    LightManager& lightManager = activeScene->getLightManager();
    const Vector<UniquePtr<Light>>& lights = lightManager.getLights();

    for (size_t i = 0; i < lights.size(); ++i)
    {
      Light* light = lights[i].get();

      ImGui::PushID(static_cast<Int32>(i));
      drawLightController(light);
      ImGui::Separator();
      ImGui::PopID();
    }
  }

  void LightManagerWindow::drawLightController(Light* light)
  {
    if (!light)
      return;

    ImGui::BeginGroup();

    // Is Enabled
    static bool enabled = light->isEnabled();
    if (ImGui::Checkbox("Enabled", &enabled))
      light->setEnabled(enabled);

    ImGui::SameLine();

    // Light Type
    const char* typeNames[] = { "Directional", "Point", "Spot" };
    lightType::Type type = light->getType();
    Int32 typeIndex = static_cast<Int32>(type);
    if (ImGui::Combo("Type", &typeIndex, typeNames, IM_ARRAYSIZE(typeNames)))
    {
      light->setType(static_cast<lightType::Type>(typeIndex));
    }

    ImGui::SameLine();

    Color color = light->getColor();
    float colorArr[3] = { color.r, color.g, color.b };
    if (ImGui::ColorEdit3("Color", colorArr, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
    {
      light->setColor(Color(colorArr[0], colorArr[1], colorArr[2]));
    }

    ImGui::EndGroup();
  }
}

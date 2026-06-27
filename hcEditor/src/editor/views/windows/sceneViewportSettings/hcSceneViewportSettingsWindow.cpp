#include "hc/editor/views/windows/sceneViewportSettings/hcSceneViewportSettingsWindow.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"

namespace hc::editor
{
  static const char* RENDER_TARGET_NAMES[] = {
    "Final Color",
    "GBuffer Position + Depth",
    "GBuffer Normal + Roughness",
    "GBuffer Albedo + Alpha",
    "GBuffer Material Parameters"
  };

  SceneViewportSettingsWindow::SceneViewportSettingsWindow(
    SceneViewportWindow& viewportWindow,
    IGraphicsManager& graphicsManager
  ) : AWindowView("Scene Viewport Settings", false, Vector2f(300.0f, 200.0f)),
    m_viewportWindow(viewportWindow),
    m_graphicsManager(graphicsManager)
  {}

  SceneViewportSettingsWindow::~SceneViewportSettingsWindow()
  {}

  void SceneViewportSettingsWindow::destroy()
  {}

  void SceneViewportSettingsWindow::onDraw()
  {
    drawViewportSettings(m_viewportWindow);
  }

  void SceneViewportSettingsWindow::drawViewportSettings(
    SceneViewportWindow& viewportWindow
  )
  {
    /*********************************/
    /* Mode and Operation Settings */

    ImGuizmo::MODE currentGizmoMode =
      static_cast<ImGuizmo::MODE>(viewportWindow.getGizmoMode());

    ImGui::Text("Gizmo Mode");
    ImGui::Separator();
    if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
      viewportWindow.setGizmoMode(ImGuizmo::LOCAL);
    ImGui::SameLine();
    if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
      viewportWindow.setGizmoMode(ImGuizmo::WORLD);

    ImGuizmo::OPERATION currentGizmoOperation =
      static_cast<ImGuizmo::OPERATION>(viewportWindow.getGizmoOperation());

    ImGui::Text("Gizmo Operation");
    ImGui::Separator();
    if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
    {
      viewportWindow.setGizmoOperation(ImGuizmo::TRANSLATE);
      currentGizmoOperation = ImGuizmo::TRANSLATE;
    }
      
    if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
    {
      viewportWindow.setGizmoOperation(ImGuizmo::ROTATE);
      currentGizmoOperation = ImGuizmo::ROTATE;
    }
      
    if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
    {
      viewportWindow.setGizmoOperation(ImGuizmo::SCALE);
      currentGizmoOperation = ImGuizmo::SCALE;
    }

    /*********************************/
    /* Snap Settings */

    bool usingSnap = viewportWindow.isUsingSnap();
    if (ImGui::Checkbox("Use Snap", &usingSnap))
      viewportWindow.setUsingSnap(usingSnap);

    Vector3f snapValues = viewportWindow.getSnapValues();
    ImGuizmo::OPERATION currentOperation =
      static_cast<ImGuizmo::OPERATION>(viewportWindow.getGizmoOperation());

    if (currentOperation == ImGuizmo::TRANSLATE)
    {
      if (ImGui::InputFloat3("Snap Translation", &snapValues.x))
        viewportWindow.setSnapValues(snapValues);
    }
    else if (currentOperation == ImGuizmo::ROTATE)
    {
      if (ImGui::InputFloat("Snap Rotation", &snapValues.x))
        viewportWindow.setSnapValues(snapValues);
    }
    else if (currentOperation == ImGuizmo::SCALE)
    {
      if (ImGui::InputFloat("Snap Scale", &snapValues.x))
        viewportWindow.setSnapValues(snapValues);
    }

    /*********************************/
    /* Render Target Selection */

    ImGui::Text("Render Target");
    ImGui::Separator();

    renderPipelineType::Type currentPipeline = m_graphicsManager.getRenderPipelineType();
    sceneViewportRenderTargetType::Type currentRenderTarget = viewportWindow.getCurrentRenderTarget();

    // If current render pipeline has changed and no longer supports GBuffer targets,
    // reset to Final Color

    if (currentPipeline != renderPipelineType::DeferredHybrid &&
      currentRenderTarget != sceneViewportRenderTargetType::FinalColor)
    {
      viewportWindow.setCurrentRenderTarget(sceneViewportRenderTargetType::FinalColor);
      currentRenderTarget = sceneViewportRenderTargetType::FinalColor;
    }

    Int32 currentRenderTargetIndex = static_cast<Int32>(currentRenderTarget);
    if (ImGui::Combo(
      "Render Target",
      &currentRenderTargetIndex,
      RENDER_TARGET_NAMES,
      IM_ARRAYSIZE(RENDER_TARGET_NAMES)
    ))
    {
      sceneViewportRenderTargetType::Type desiredRenderTarget =
        static_cast<sceneViewportRenderTargetType::Type>(currentRenderTargetIndex);

      if (desiredRenderTarget == sceneViewportRenderTargetType::FinalColor)
      {
        viewportWindow.setCurrentRenderTarget(desiredRenderTarget);
      }
      else if (currentPipeline != renderPipelineType::DeferredHybrid)
      {
        // If user selects a GBuffer target but the current render pipeline does not support
        // it, show an error and do not change the selection

        viewportWindow.setCurrentRenderTarget(sceneViewportRenderTargetType::FinalColor);
        LogService::Error(
          "SceneViewportSettingsWindow: Cannot switch to GBuffer render target because the current render pipeline does not support it."
        );
      }
      else
      {
        viewportWindow.setCurrentRenderTarget(desiredRenderTarget);
      }
    }

    /*********************************/
    /* Misc Settings */

    ImGui::Text("Miscellaneous");
    ImGui::Separator();

    bool drawingGrid = viewportWindow.isDrawingGrid();
    if (ImGui::Checkbox("Draw Grid", &drawingGrid))
      viewportWindow.setDrawingGrid(drawingGrid);

    if (drawingGrid)
    {
      float gridSize = viewportWindow.getGridSize();
      if (ImGui::InputFloat("Grid Size", &gridSize))
        viewportWindow.setGridSize(gridSize);
    }
  }
}

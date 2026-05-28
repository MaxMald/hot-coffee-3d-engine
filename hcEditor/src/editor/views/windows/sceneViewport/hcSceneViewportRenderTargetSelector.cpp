#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderTargetSelector.h"
#include <imgui.h>

namespace hc::editor
{
  static const char* RENDER_TARGET_NAMES[] = {
    "Final Color",
    "GBuffer Position + Depth",
    "GBuffer Normal + Roughness",
    "GBuffer Albedo + Alpha",
    "GBuffer Material Parameters"
  };

  SceneViewportRenderTargetSelector::SceneViewportRenderTargetSelector(
    sceneViewportRenderTargetType::Type initialRenderTarget,
    IGraphicsManager& graphicsManager
  ) :
    m_currentRenderTarget(initialRenderTarget),
    m_graphicsManager(graphicsManager)
  {}

  SceneViewportRenderTargetSelector::~SceneViewportRenderTargetSelector()
  {}

  sceneViewportRenderTargetType::Type SceneViewportRenderTargetSelector::getCurrentRenderTarget() const
  {
    return m_currentRenderTarget;
  }

  void SceneViewportRenderTargetSelector::draw()
  {
    renderPipelineType::Type currentPipeline = m_graphicsManager.getRenderPipelineType();

    // If current render pipeline has changed and no longer supports GBuffer targets,
    // reset to Final Color

    if (currentPipeline != renderPipelineType::DeferredHybrid &&
      m_currentRenderTarget != sceneViewportRenderTargetType::FinalColor)
    {
      m_currentRenderTarget = sceneViewportRenderTargetType::FinalColor;
    }

    Int32 currentRenderTargetIndex = static_cast<Int32>(m_currentRenderTarget);
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
        m_currentRenderTarget = desiredRenderTarget;
        return;
      }

      if (currentPipeline != renderPipelineType::DeferredHybrid)
      {
        // If user selects a GBuffer target but the current render pipeline does not support
        // it, show an error and do not change the selection

        LogService::Error(
          "SceneViewportWindow: Cannot switch to GBuffer render target because the current render pipeline does not support it."
        );
        return;
      }
      else
      {
        m_currentRenderTarget = desiredRenderTarget;
      }
    }
  }
}

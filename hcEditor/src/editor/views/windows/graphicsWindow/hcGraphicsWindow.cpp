#include "hc/editor/views/windows/graphicsWindow/hcGraphicsWindow.h"

namespace hc::editor
{
  GraphicsWindow::GraphicsWindow(IGraphicsManager& graphicsManager) :
    AWindowView("Graphics"),
    m_graphicsManager(graphicsManager)
  {}

  GraphicsWindow::~GraphicsWindow()
  {}

  void GraphicsWindow::destroy()
  {}

  void GraphicsWindow::onDraw()
  {
    ImGui::Text("Render Pipeline");
    ImGui::Separator();

    renderPipelineType::Type currentPipeline = m_graphicsManager.getRenderPipelineType();

    if (ImGui::RadioButton("Forward", currentPipeline == renderPipelineType::Forward))
      m_graphicsManager.setRenderPipelineType(renderPipelineType::Forward);
    if (ImGui::RadioButton("Deferred Hybrid", currentPipeline == renderPipelineType::DeferredHybrid))
      m_graphicsManager.setRenderPipelineType(renderPipelineType::DeferredHybrid);
  }
}

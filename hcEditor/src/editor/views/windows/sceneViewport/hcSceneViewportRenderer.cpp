#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderer.h"

namespace hc::editor
{
  SceneViewportRenderer::SceneViewportRenderer(IGraphicsManager& graphicsManager) :
    m_graphicsManager(graphicsManager),
    m_frameBuffer(nullptr),
    m_clearColor(Color::Black())
  {
    m_frameBuffer = m_graphicsManager.createFrameBuffer(10, 10);
  }

  SceneViewportRenderer::~SceneViewportRenderer()
  {
  }

  void SceneViewportRenderer::resize(UInt32 width, UInt32 height)
  {
    m_frameBuffer->resize(width, height);
  }

  UInt32 SceneViewportRenderer::getWidth() const
  {
    return m_frameBuffer->getWidth();
  }

  UInt32 SceneViewportRenderer::getHeight() const
  {
    return m_frameBuffer->getHeight();
  }

  void SceneViewportRenderer::renderScene(Scene& scene, Camera& camera)
  {
    m_frameBuffer->bind();
    m_frameBuffer->clear(m_clearColor);
    scene.draw(&(camera));
    m_graphicsManager.executeDrawCommands();
    m_frameBuffer->unbind();
  }

  ITexture& SceneViewportRenderer::getRenderedTexture() const
  {
    return m_frameBuffer->getColorTexture();
  }

  const Color& SceneViewportRenderer::getClearColor() const
  {
    return m_clearColor;
  }

  void SceneViewportRenderer::setClearColor(const Color& color)
  {
    m_clearColor = color;
  }

  bool SceneViewportRenderer::isValid() const
  {
    return m_frameBuffer && m_frameBuffer->isValid();
  }
}

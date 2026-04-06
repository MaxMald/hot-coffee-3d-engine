#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderer.h"

namespace hc::editor
{
  SceneViewportRenderer::SceneViewportRenderer(IGraphicsManager& graphicsManager) :
    m_graphicsManager(graphicsManager),
    m_frameBuffer(nullptr),
    m_clearColor(Color::Black())
  {
    try
    {
      m_frameBuffer = m_graphicsManager.createFrameBuffer(10, 10);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to create framebuffer for scene viewport renderer: " + String(e.what())
      );
      m_frameBuffer = nullptr;
    }
  }

  SceneViewportRenderer::~SceneViewportRenderer()
  {
  }

  void SceneViewportRenderer::resize(UInt32 width, UInt32 height)
  {
    assertFrameBufferValid();
    m_frameBuffer->resize(width, height);
  }

  UInt32 SceneViewportRenderer::getWidth() const
  {
    assertFrameBufferValid();
    return m_frameBuffer->getWidth();
  }

  UInt32 SceneViewportRenderer::getHeight() const
  {
    assertFrameBufferValid();
    return m_frameBuffer->getHeight();
  }

  void SceneViewportRenderer::renderScene(Scene& scene, Camera& camera)
  {
    assertFrameBufferValid();
    m_frameBuffer->bind();
    m_frameBuffer->clear(m_clearColor);
    scene.draw(&(camera));
    m_graphicsManager.executeDrawCommands();
    m_frameBuffer->unbind();
  }

  ITexture& SceneViewportRenderer::getRenderedTexture() const
  {
    assertFrameBufferValid();
    return m_frameBuffer->getColorTexture();
  }

  const Color& SceneViewportRenderer::getClearColor() const
  {
    assertFrameBufferValid();
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

  void SceneViewportRenderer::assertFrameBufferValid() const
  {
    if (!m_frameBuffer || !m_frameBuffer->isValid())
    {
      throw Exception("SceneViewportRenderer framebuffer is not valid.");
    }
  }
}

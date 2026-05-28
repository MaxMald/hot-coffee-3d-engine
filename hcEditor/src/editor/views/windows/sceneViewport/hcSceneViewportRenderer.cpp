#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderer.h"

namespace hc::editor
{
  SceneViewportRenderer::SceneViewportRenderer(
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    m_graphicsManager(graphicsManager),
    m_frameBuffer(nullptr),
    m_clearColor(Color::Black()),
    m_isPrepared(false),
    m_lightGizmoRenderer(assetManager, graphicsManager)
  {}

  SceneViewportRenderer::~SceneViewportRenderer()
  {}

  void SceneViewportRenderer::prepare()
  {
    if (m_isPrepared)
      return;

    try
    {
      m_frameBuffer = m_graphicsManager.createFrameBuffer(10, 10);
      m_lightGizmoRenderer.prepare();
      m_isPrepared = true;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to create framebuffer for scene viewport renderer: " + String(e.what())
      );
      m_frameBuffer = nullptr;
    }
  }

  void SceneViewportRenderer::resize(UInt32 width, UInt32 height)
  {
    assertIsValid();
    m_frameBuffer->resize(width, height);
  }

  UInt32 SceneViewportRenderer::getWidth() const
  {
    assertIsValid();
    return m_frameBuffer->getWidth();
  }

  UInt32 SceneViewportRenderer::getHeight() const
  {
    assertIsValid();
    return m_frameBuffer->getHeight();
  }

  void SceneViewportRenderer::renderScene(Scene& scene, Camera& camera)
  {
    assertIsValid();
    m_frameBuffer->bind();
    m_frameBuffer->clear(m_clearColor);
    scene.draw(m_graphicsManager, &camera);
    m_graphicsManager.executeDrawCommands();
    m_frameBuffer->unbind();
  }

  void SceneViewportRenderer::renderLightGizmos(
    const Scene& scene,
    const Camera& camera,
    const GameObject* activeGameObject)
  {
    assertIsValid();
    m_frameBuffer->bind();
    m_lightGizmoRenderer.draw(scene, camera, activeGameObject);
    m_graphicsManager.executeDrawCommands();
    m_frameBuffer->unbind();
  }

  ITexture& SceneViewportRenderer::getRenderedTexture() const
  {
    assertIsValid();
    return m_frameBuffer->getColorTexture();
  }

  const Color& SceneViewportRenderer::getClearColor() const
  {
    assertIsValid();
    return m_clearColor;
  }

  void SceneViewportRenderer::setClearColor(const Color& color)
  {
    m_clearColor = color;
  }

  bool SceneViewportRenderer::isValid() const
  {
    return m_frameBuffer && m_frameBuffer->isValid() && m_isPrepared;
  }

  void SceneViewportRenderer::assertIsValid() const
  {
    if (!m_frameBuffer || !m_frameBuffer->isValid() || !m_isPrepared)
    {
      throw Exception("SceneViewportRenderer framebuffer is not valid.");
    }
  }
}

#include "hc/editor/views/windows/sceneViewport/hcSceneViewportRenderer.h"

namespace hc::editor
{
  static constexpr UInt32 MIN_FRAMEBUFFER_SIZE = 10;

  SceneViewportRenderer::SceneViewportRenderer(
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager
  ) :
    m_graphicsManager(graphicsManager),
    m_frameBuffer(nullptr),
    m_clearColor(Color::Black()),
    m_isPrepared(false),
    m_lightGizmoRenderer(assetManager, graphicsManager),
    m_width(MIN_FRAMEBUFFER_SIZE),
    m_height(MIN_FRAMEBUFFER_SIZE)
  {}

  SceneViewportRenderer::~SceneViewportRenderer()
  {}

  void SceneViewportRenderer::prepare()
  {
    if (m_isPrepared)
      return;

    try
    {
      m_frameBuffer = m_graphicsManager.createFrameBuffer(
        MIN_FRAMEBUFFER_SIZE,
        MIN_FRAMEBUFFER_SIZE
      );
      m_width = MIN_FRAMEBUFFER_SIZE;
      m_height = MIN_FRAMEBUFFER_SIZE;
      m_graphicsManager.setRenderTarget(m_frameBuffer.get());
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

    if (width == m_width && height == m_height)
      return;

    width = Math::Max(width, MIN_FRAMEBUFFER_SIZE);
    height = Math::Max(height, MIN_FRAMEBUFFER_SIZE);

    m_frameBuffer->resize(width, height);
    m_graphicsManager.setViewport(Rect<UInt32>(0, 0, width, height));
    m_width = width;
    m_height = height;
  }

  UInt32 SceneViewportRenderer::getWidth() const
  {
    return m_width;
  }

  UInt32 SceneViewportRenderer::getHeight() const
  {
    return m_height;
  }

  void SceneViewportRenderer::renderScene(
    Scene& scene,
    Camera& camera,
    const GameObject* activeGameObject
  )
  {
    assertIsValid();

    m_frameBuffer->clear(m_clearColor);

    // Add draw commands for light gizmos to the scene's draw commands
    m_lightGizmoRenderer.draw(scene, camera, activeGameObject);

    // Render the scene to the framebuffer
    scene.draw(m_graphicsManager, &camera);
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

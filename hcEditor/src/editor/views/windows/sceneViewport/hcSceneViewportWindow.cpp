#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  SceneViewportWindow::SceneViewportWindow(HotCoffeeEngine& engine) :
    AWindowView("Scene Viewport"),
    m_engine(engine),
    m_frameBuffer(nullptr),
    m_uvTopLeft(0, 1),
    m_uvBottomRight(1, 0)
  {
    m_frameBuffer = m_engine
      .getGraphicsManager()
      .createFrameBuffer(10, 10);

    if (!m_frameBuffer)
      throw RuntimeErrorException(
        "SceneViewportWindow: Failed to create framebuffer for scene viewport."
      );

    graphicsBackendType::Type backendType = m_engine
      .getGraphicsManager()
      .getGraphicsBackendType();

    if (backendType == graphicsBackendType::OPENGL)
    {
      // OpenGL's texture coordinate system is flipped vertically compared to ImGui's,
      // so we need to flip the UVs when using OpenGL.

      m_uvTopLeft = Vector2f(0, 1);
      m_uvBottomRight = Vector2f(1, 0);
    }
  }

  void SceneViewportWindow::onDraw()
  {
    updateFramebufferSize();
    renderSceneToTexture();
    drawViewport();
  }

  void SceneViewportWindow::updateFramebufferSize()
  {
    Vector2f contentSize = getContentSize();
    UInt32 width = static_cast<UInt32>(contentSize.x);
    UInt32 height = static_cast<UInt32>(contentSize.y);

    if (width == 0 || height == 0)
    {
      LogService::Error("SceneViewportWindow: Cannot resize framebuffer to zero dimensions.");
      return;
    }

    if (!m_frameBuffer)
    {
      throw RuntimeErrorException(
        "SceneViewportWindow: Framebuffer is not initialized. Cannot resize."
      );
    }

    m_frameBuffer->resize(width, height);
  }

  void SceneViewportWindow::renderSceneToTexture()
  {
    Scene* contentScene = m_engine
      .getSceneManager()
      .getScene(EditorSceneNames::CONTENT_SCENE);

    if (!contentScene)
    {
      LogService::Error("SceneViewportWindow: Content scene not found. Cannot render to texture.");
      return;
    }

    if (!m_frameBuffer || !m_frameBuffer->isValid())
    {
      throw RuntimeErrorException(
        "SceneViewportWindow: Invalid frame buffer."
      );
    }

    m_frameBuffer->bind();
    m_frameBuffer->clear(Color(0.1f, 0.1f, 0.1f, 1.0f));
    contentScene->draw();
    m_engine.getGraphicsManager().executeDrawCommands();
    m_frameBuffer->unbind();
  }

  void SceneViewportWindow::drawViewport()
  {
    if (!m_frameBuffer || !m_frameBuffer->isValid())
    {
      throw RuntimeErrorException(
        "SceneViewportWindow: Invalid frame buffer."
      );
    }

    Vector2f contentSize = getContentSize();
    imguiUtilities::DrawTexture(
      &(m_frameBuffer->getColorTexture()),
      contentSize.x,
      contentSize.y,
      m_uvTopLeft,
      m_uvBottomRight
    );
  }
}

#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"
#include "hc/editor/imgui/hcImguiUtilities.h"

namespace hc::editor
{
  SceneViewportWindow::SceneViewportWindow(HotCoffeeEngine& engine) :
    AWindowView("Scene Viewport", true),
    m_engine(engine),
    m_renderer(m_engine.getGraphicsManager()),
    m_cameraController(m_engine.getInputManager()),
    m_uvTopLeft(0, 1),
    m_uvBottomRight(1, 0)
  {
    graphicsBackendType::Type backendType = m_engine
      .getGraphicsManager()
      .getGraphicsBackendType();

    if (backendType == graphicsBackendType::OPENGL)
    {
      m_uvTopLeft = Vector2f(0, 1); // flipped vertically 
      m_uvBottomRight = Vector2f(1, 0);
    }

    m_cameraController.prepare();

    if (!m_renderer.isValid())
    {
      LogService::Error(
        "SceneViewportWindow: Failed to initialize renderer. Framebuffer is invalid."
      );
    }
  }

  void SceneViewportWindow::destroy()
  {
  }

  void SceneViewportWindow::onUpdate(const Time& elapsedTime)
  {
    if (isFocused())
      m_cameraController.update(elapsedTime);
  }

  void SceneViewportWindow::onDraw()
  {
    if (!m_renderer.isValid())
      return;

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

    m_renderer.resize(width, height);
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

    m_renderer.renderScene(
      *contentScene,
      m_cameraController.getCamera()
    );
  }

  void SceneViewportWindow::drawViewport()
  {
    imguiUtilities::DrawTexture(
      &(m_renderer.getRenderedTexture()),
      static_cast<float>(m_renderer.getWidth()),
      static_cast<float>(m_renderer.getHeight()),
      m_uvTopLeft,
      m_uvBottomRight
    );
  }
}

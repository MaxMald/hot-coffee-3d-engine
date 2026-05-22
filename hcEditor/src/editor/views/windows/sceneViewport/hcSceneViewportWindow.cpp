#include "hc/editor/views/windows/sceneViewport/hcSceneViewportWindow.h"
#include "hc/editor/scenes/hcEditorSceneNames.h"
#include "hc/editor/imgui/hcImguiUtilities.h"
#include <imgui.h>

namespace hc::editor
{
  SceneViewportWindow::SceneViewportWindow(
    HotCoffeeEngine& engine,
    GameObjectSelectionService& selectionService
  ) :
    AWindowView("Scene Viewport", true),
    m_engine(engine),
    m_selectionService(selectionService),
    m_renderer(m_engine.getAssetManager(), m_engine.getGraphicsManager()),
    m_cameraController(m_engine.getInputManager()),
    m_gizmoController(m_engine.getInputManager(), m_cameraController.getCamera()),
    m_renderTargetSelector(sceneViewportRenderTargetType::FinalColor, m_engine.getGraphicsManager()),
    m_uvTopLeft(0, 0),
    m_uvBottomRight(1, 1)
  {
    graphicsBackendType::Type backendType = m_engine
      .getGraphicsManager()
      .getGraphicsBackendType();

    if (backendType == graphicsBackendType::OPENGL)
    {
      // flipped vertically 
      m_uvTopLeft = Vector2f(0, 1);
      m_uvBottomRight = Vector2f(1, 0);
    }

    m_renderer.prepare();
    m_cameraController.prepare();

    if (!m_renderer.isValid())
    {
      LogService::Error(
        "SceneViewportWindow: Failed to initialize renderer. Framebuffer is invalid."
      );
      return;
    }

    m_selectionService.subscribe(this);
    if (m_selectionService.hasSelectedGameObjects())
    {
      m_gizmoController.setActiveGameObject(
        m_selectionService.getFirstSelectedGameObject()
      );
    }
  }

  void SceneViewportWindow::destroy()
  {
    m_selectionService.unsubscribe(this);
  }

  void SceneViewportWindow::onUpdate(const Time& elapsedTime)
  {
    if (isFocused())
    {
      m_cameraController.update(elapsedTime);
      m_gizmoController.update(elapsedTime);
    }
  }

  void SceneViewportWindow::onDraw()
  {
    if (!m_renderer.isValid())
      return;

    updateFramebufferSize();
    renderSceneToTexture();
    renderGizmosToTexture();
    m_renderTargetSelector.draw();

    ImVec2 viewportPos = ImGui::GetCursorScreenPos();

    sceneViewportRenderTargetType::Type currentRenderTarget =
      m_renderTargetSelector.getCurrentRenderTarget();

    if (currentRenderTarget == sceneViewportRenderTargetType::FinalColor)
    {
      drawRenderTarget(m_renderer.getRenderedTexture());
    }
    else
    {
      const IGBuffer& gBuffer = m_engine.getGraphicsManager().getGBuffer();

      if (currentRenderTarget == sceneViewportRenderTargetType::GBufferPosition)
      {
        drawRenderTarget(gBuffer.getPosition());
      }
      else if (currentRenderTarget == sceneViewportRenderTargetType::GBufferNormalRoughness)
      {
        drawRenderTarget(gBuffer.getNormalRoughness());
      }
      else if (currentRenderTarget == sceneViewportRenderTargetType::GBufferAlbedoAlpha)
      {
        drawRenderTarget(gBuffer.getAlbedoAlpha());
      }
      else if (currentRenderTarget == sceneViewportRenderTargetType::GBufferMaterialParameters)
      {
        drawRenderTarget(gBuffer.getMaterialParameters());
      }
    }
    
    m_gizmoController.draw(
      Vector2f(viewportPos.x, viewportPos.y),
      getContentSize()
    );
  }

  void SceneViewportWindow::onGameObjectSelected(GameObject* gameObject)
  {
    m_gizmoController.setActiveGameObject(gameObject);
  }

  void SceneViewportWindow::onGameObjectDeselected(GameObject* gameObject)
  {
    if (!m_gizmoController.hasActiveGameObject())
      return;

    if (m_gizmoController.getActiveGameObject() == gameObject)
      m_gizmoController.clearActiveGameObject();
  }

  void SceneViewportWindow::updateFramebufferSize()
  {
    Vector2f contentSize = getContentSize();
    UInt32 width = static_cast<UInt32>(contentSize.x);
    UInt32 height = static_cast<UInt32>(contentSize.y);

    if (width == 0 || height == 0)
      return;

    m_renderer.resize(width, height);
    m_cameraController.getCamera().setAspectRatio(width, height);
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

  void SceneViewportWindow::renderGizmosToTexture()
  {
    Scene* contentScene = m_engine
      .getSceneManager()
      .getScene(EditorSceneNames::CONTENT_SCENE);

    if (!contentScene)
    {
      LogService::Error("SceneViewportWindow: Content scene not found. Cannot render to texture.");
      return;
    }

    m_renderer.renderLightGizmos(
      *contentScene,
      m_cameraController.getCamera(),
      m_gizmoController.getActiveGameObject()
    );
  }

  void SceneViewportWindow::drawRenderTarget(const ITexture& texture)
  {
    imguiUtilities::DrawTexture(
      &texture,
      static_cast<float>(texture.getWidth()),
      static_cast<float>(texture.getHeight()),
      m_uvTopLeft,
      m_uvBottomRight
    );
  }
}

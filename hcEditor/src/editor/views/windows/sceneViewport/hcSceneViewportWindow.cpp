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
    m_camera(),
    m_renderer(m_engine.getAssetManager(), m_engine.getGraphicsManager()),
    m_cameraInputController(m_camera, m_engine.getInputManager()),
    m_gizmoController(m_engine.getInputManager(), m_camera),
    m_uvTopLeft(0, 0),
    m_uvBottomRight(1, 1),
    m_currentRenderTarget(sceneViewportRenderTargetType::FinalColor)
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
    m_camera.setCameraPosition(Vector3f(0.0f, 0.0f, 5.0f));

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

  UInt32 SceneViewportWindow::getGizmoMode() const
  {
    return m_gizmoController.getGizmoMode();
  }

  void SceneViewportWindow::setGizmoMode(UInt32 mode)
  {
    m_gizmoController.setGizmoMode(mode);
  }

  UInt32 SceneViewportWindow::getGizmoOperation() const
  {
    return m_gizmoController.getGizmoOperation();
  }

  void SceneViewportWindow::setGizmoOperation(UInt32 operation)
  {
    m_gizmoController.setGizmoOperation(operation);
  }

  bool SceneViewportWindow::isDrawingGrid() const
  {
    return m_gizmoController.isDrawingGrid();
  }

  void SceneViewportWindow::setDrawingGrid(bool drawingGrid)
  {
    m_gizmoController.setDrawingGrid(drawingGrid);
  }

  bool SceneViewportWindow::isUsingSnap() const
  {
    return m_gizmoController.isUsingSnap();
  }

  void SceneViewportWindow::setUsingSnap(bool usingSnap)
  {
    m_gizmoController.setUsingSnap(usingSnap);
  }

  float SceneViewportWindow::getGridSize() const
  {
    return m_gizmoController.getGridSize();
  }

  void SceneViewportWindow::setGridSize(float gridSize)
  {
    m_gizmoController.setGridSize(gridSize);
  }

  Vector3f SceneViewportWindow::getSnapValues() const
  {
    return m_gizmoController.getSnapValues();
  }

  void SceneViewportWindow::setSnapValues(const Vector3f& snapValues)
  {
    m_gizmoController.setSnapValues(snapValues);
  }

  sceneViewportRenderTargetType::Type SceneViewportWindow::getCurrentRenderTarget() const
  {
    return m_currentRenderTarget;
  }

  void SceneViewportWindow::setCurrentRenderTarget(sceneViewportRenderTargetType::Type renderTarget)
  {
    m_currentRenderTarget = renderTarget;
  }

  SceneViewportCamera& SceneViewportWindow::getViewportCamera()
  {
    return m_camera;
  }

  void SceneViewportWindow::destroy()
  {
    m_selectionService.unsubscribe(this);
  }

  void SceneViewportWindow::onUpdate(const Time& elapsedTime)
  {
    if (isFocused())
    {
      m_cameraInputController.update(elapsedTime);
      m_gizmoController.update(elapsedTime);
    }
  }

  void SceneViewportWindow::onDraw()
  {
    if (!m_renderer.isValid())
      return;

    updateFramebufferSize();
    renderSceneToTexture();

    ImVec2 viewportPos = ImGui::GetCursorScreenPos();

    if (m_currentRenderTarget == sceneViewportRenderTargetType::FinalColor)
    {
      drawRenderTarget(m_renderer.getRenderedTexture());
    }
    else
    {
      const IGBuffer& gBuffer = m_engine.getGraphicsManager().getGBuffer();

      if (m_currentRenderTarget == sceneViewportRenderTargetType::GBufferPositionAndDepth)
      {
        drawRenderTarget(gBuffer.getPositionAndDepth());
      }
      else if (m_currentRenderTarget == sceneViewportRenderTargetType::GBufferNormalRoughness)
      {
        drawRenderTarget(gBuffer.getNormalRoughness());
      }
      else if (m_currentRenderTarget == sceneViewportRenderTargetType::GBufferAlbedoAlpha)
      {
        drawRenderTarget(gBuffer.getAlbedoAlpha());
      }
      else if (m_currentRenderTarget == sceneViewportRenderTargetType::GBufferMaterialParameters)
      {
        drawRenderTarget(gBuffer.getMaterialParameters());
      }
      else if (m_currentRenderTarget == sceneViewportRenderTargetType::GBufferSpecularColorAndShininess)
      {
        drawRenderTarget(gBuffer.getSpecularColorAndShininess());
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
    m_camera.getCamera().setAspectRatio(width, height);
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
      m_camera.getCamera(),
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

#include "hc/editor/scenes/hcEditorScene.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistory.h"
#include "hc/editor/services/hcEditorServiceManagerRegistry.h"
#include "hc/editor/views/hcEditorViewsRegistry.h"

namespace hc::editor
{
  EditorScene::EditorScene(
    HotCoffeeEngine& engine,
    EditorLogHistory& editorLogHistory
  ) :
    Scene(),
    m_engine(engine),
    m_editorLogHistory(editorLogHistory),
    m_serviceManager(),
    m_viewsManager()
  {
  }

  EditorScene::~EditorScene()
  {
  }

  void EditorScene::onInitialized()
  {
    editorServiceManagerRegistry::registerServices(m_engine, m_serviceManager);
    m_viewsManager.initialize(m_engine.getWindowManager().getWindow());
    m_engine.addEventListener(&m_viewsManager);
    editorViewsRegistry::registerDefaultViews(
      m_engine,
      m_viewsManager,
      m_serviceManager,
      m_editorLogHistory
    );
  }

  void EditorScene::onActivate()
  {
    m_serviceManager.prepareServices();
  }

  void EditorScene::onAfterDraw(const RenderContext&)
  {
    m_viewsManager.draw(
      m_engine.getWindowManager().getWindow(),
      m_engine.getElapsedTime()
    );
  }

  void EditorScene::onBeforeUpdate(const Time& elapsedTime)
  {
    m_serviceManager.update(elapsedTime);
  }

  void EditorScene::onDestroy()
  {
    m_engine.removeEventListener(&m_viewsManager);
    m_viewsManager.destroy();
    m_serviceManager.clear();
  }
}

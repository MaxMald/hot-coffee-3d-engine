#include "hc/editor/views/hcEditorViewsManager.h"

#include "hc/editor/views/hcIView.h"
#include "hc/editor/imgui/hcImguiHandler.h"
#include "hc/editor/views/hcProjectFileSelectorView.h"

namespace hc::editor
{
  EditorViewsManager::EditorViewsManager(HotCoffeeEngine& engine) :
    m_initialized(false),
    m_engine(engine)
  { 
  }

  void EditorViewsManager::initialize()
  {
    if (m_initialized)
      return;

    m_engine.addGameLoopListener(this);
    hcImguiHandler::init(m_engine.getWindowManager().getWindow());
    m_initialized = true;
  }

  bool EditorViewsManager::processEvent(const Event& event)
  {
    return hcImguiHandler::processEvent(event);
  }

  void EditorViewsManager::registerView(UniquePtr<IView> view)
  {
    m_views.push_back(std::move(view));
  }

  void EditorViewsManager::clear()
  {
    for (const UniquePtr<IView>& view : m_views)
      view->destroy();
    m_views.clear();
  }

  void EditorViewsManager::destroy()
  {
    if (!m_initialized)
      return;

    clear();
    hcImguiHandler::destroy();
    m_engine.removeGameLoopListener(this);
    m_initialized = false;
  }

  bool EditorViewsManager::onEvent(const Event& event)
  {
    return hcImguiHandler::processEvent(event);
  }

  void EditorViewsManager::onAfterSceneRender()
  {
    hcImguiHandler::beginFrame(
      m_engine.getWindowManager().getWindow(),
      m_engine.getElapsedTime()
    );

    for (const UniquePtr<IView>& view : m_views)
      view->draw();

    hcImguiHandler::endFrame();
  }
}

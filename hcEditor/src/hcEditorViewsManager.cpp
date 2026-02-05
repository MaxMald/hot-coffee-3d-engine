#include "hc/editor/hcEditorViewsManager.h"

#include "hc/editor/hcIView.h"
#include "hc/editor/hcImguiHandler.h"
#include "hc/editor/hcProjectFileSelector.h"

namespace hc::editor
{
  EditorViewsManager::EditorViewsManager() :
    m_initialized(false)
  {
  }

  void EditorViewsManager::initialize(IWindow& window)
  {
    if (m_initialized)
      return;

    hcImguiHandler::init(window);
    m_initialized = true;
  }

  bool EditorViewsManager::processEvent(const Event& event)
  {
    return hcImguiHandler::processEvent(event);
  }

  void EditorViewsManager::draw()
  {
    hcImguiHandler::beginFrame();
    for (const UniquePtr<IView>& view : m_views)
      view->draw();
    hcImguiHandler::endFrame();
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
}

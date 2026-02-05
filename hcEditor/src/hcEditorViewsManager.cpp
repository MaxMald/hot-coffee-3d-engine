#include "hc/editor/hcEditorViewsManager.h"

#include "hc/editor/hcIView.h"
#include "hc/editor/hcImguiHandler.h"

namespace hc::editor
{
  void EditorViewsManager::initialize()
  {
    hcImguiHandler::init(
      HotCoffeeEngine::Instance().getWindowManager().getWindow()
    );
  }

  bool EditorViewsManager::processEvent(const Event& event)
  {
    return hcImguiHandler::processEvent(event);
  }

  void EditorViewsManager::draw()
  {
    hcImguiHandler::beginFrame();
    for (IView* view : m_views)
      view->draw();
    hcImguiHandler::endFrame();
  }

  void EditorViewsManager::registerView(IView* view)
  {
    m_views.push_back(view);
  }

  void EditorViewsManager::unregisterView(IView* view)
  {
    m_views.erase(
      std::remove(m_views.begin(), m_views.end(), view),
      m_views.end()
    );
  }

  void EditorViewsManager::destroy()
  {
    for (IView* view : m_views)
      view->destroy();
    m_views.clear();

    hcImguiHandler::destroy();
  }
}

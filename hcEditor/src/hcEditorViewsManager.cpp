#include "hc/editor/hcEditorViewsManager.h"

#include "hc/editor/hcIView.h"
#include "hc/editor/hcImguiHandler.h"

namespace hc::editor
{
  EditorViewsManager::EditorViewsManager()
  {
  }

  EditorViewsManager::~EditorViewsManager()
  {
  }

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

  void EditorViewsManager::shutdown()
  {
    clearViews();
    hcImguiHandler::destroy();
  }

  void EditorViewsManager::registerView(IView* view)
  {
    m_views.push_back(view);
  }

  void EditorViewsManager::clearViews()
  {
    m_views.clear();
  }
}

#include "hc/editor/hcEditorViewsManager.h"

#include "hc/editor/hcIView.h"
#include "hc/editor/hcImguiHandler.h"

namespace hc::editor
{
  EditorViewsManager* EditorViewsManager::s_instance = nullptr;

  EditorViewsManager& EditorViewsManager::Instance()
  {
    if (!s_instance)
    {
      throw RuntimeErrorException(
        "EditorViewsManager instance is not prepared. Call Prepare() before accessing the instance."
      );
    }
    return *s_instance;
  }

  void EditorViewsManager::Prepare()
  {
    if (!s_instance)
    {
      s_instance = new EditorViewsManager();
    }
  }

  void EditorViewsManager::Shutdown()
  {
    if (s_instance)
    {
      s_instance->destroy();
      delete s_instance;
      s_instance = nullptr;
    }
  }

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

  void EditorViewsManager::clearViews()
  {
    for (IView* view : m_views)
      view->destroy();
    m_views.clear();
  }

  void EditorViewsManager::destroy()
  {
    clearViews();
    hcImguiHandler::destroy();
  }
}

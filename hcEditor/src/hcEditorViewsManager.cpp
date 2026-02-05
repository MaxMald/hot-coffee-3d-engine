#include "hc/editor/hcEditorViewsManager.h"

#include "hc/editor/hcIView.h"
#include "hc/editor/hcImguiHandler.h"
#include "hc/editor/hcEditorViewsInitializer.h"
#include "hc/editor/hcComponentViewManager.h"
#include "hc/editor/hcProjectFileSelector.h"

namespace hc::editor
{
  void EditorViewsManager::Initialize()
  {
    EditorViewsManager::Instance().initialize();
  }

  bool EditorViewsManager::ProcessEvent(const Event& event)
  {
    return EditorViewsManager::Instance().processEvent(event);
  }

  void EditorViewsManager::Draw()
  {
    EditorViewsManager::Instance().draw();
  }

  EditorViewsManager::EditorViewsManager() :
    m_initialized(false)
  {
  }

  void EditorViewsManager::initialize()
  {
    if (m_initialized)
      return;

    hcImguiHandler::init(HotCoffeeEngine::GetWindowManager().getWindow());

    ComponentViewManager::Prepare();
    editorViewsInitializer::registerDefaultViews();

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

  void EditorViewsManager::onPrepare()
  {
    // Intentionally left blank
  }

  void EditorViewsManager::onShutdown()
  {
    for (const UniquePtr<IView>& view : m_views)
      view->destroy();
    m_views.clear();

    if (m_initialized)
    {
      ComponentViewManager::Shutdown();

      hcImguiHandler::destroy();
      m_initialized = false;
    }
  }
}

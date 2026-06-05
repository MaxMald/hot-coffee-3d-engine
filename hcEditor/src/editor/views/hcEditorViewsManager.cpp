#include "hc/editor/views/hcEditorViewsManager.h"

#include "hc/editor/views/hcIView.h"
#include "hc/editor/imgui/hcImguiHandler.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  EditorViewsManager::EditorViewsManager() :
    m_initialized(false),
    m_window(nullptr),
    m_views()
  { 
  }

  void EditorViewsManager::initialize(IWindow* window)
  {
    if (m_initialized)
      return;

    if (!window)
      throw InvalidArgumentException("Window pointer cannot be null.");

    hcImguiHandler::init(*window);
    m_window = window;
    m_initialized = true;
  }

  bool EditorViewsManager::processEvent(const Event& event)
  {
    return hcImguiHandler::processEvent(event);
  }

  void EditorViewsManager::update(const Time& elapsedTime)
  {
    if (!m_initialized)
      throw RuntimeErrorException("Cannot update views: EditorViewsManager is not initialized.");

    for (const UniquePtr<IView>& view : m_views)
      view->update(elapsedTime);
  }

  void EditorViewsManager::draw(const Time& elapsedTime)
  {
    if (!m_initialized)
      throw RuntimeErrorException("Cannot draw views: EditorViewsManager is not initialized.");

    if (!m_window)
      throw RuntimeErrorException("Cannot draw views: Window is not initialized.");

    hcImguiHandler::beginFrame(*m_window, elapsedTime);
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

  void EditorViewsManager::destroy()
  {
    if (!m_initialized)
      return;

    clear();
    hcImguiHandler::destroy();
    m_window = nullptr;
    m_initialized = false;
  }
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/views/hcIView.h"

namespace hc::editor
{
  /**
   * @brief Manages editor views and their lifecycle in the editor.
   */
  class EditorViewsManager : public IEventListener
  {
  public:
    EditorViewsManager();
    virtual ~EditorViewsManager() = default;

    /**
     * @brief Initialize the manager and register default views.
     *
     * @param window Reference to the main application window.
     */
    void initialize(IWindow& window);

    /**
     * @brief Retrieve a view of the specified type.
     *
     * @tparam T The view type to retrieve. Must derive from IView.
     *
     * @return Pointer to the view if found, nullptr otherwise.
     */
    template<typename T>
    T* getView() const;

    /**
     * @brief Process an event for all registered views.
     *
     * @param event The event to process.
     *
     * @return True if the event was handled by any view, false otherwise.
     */
    bool processEvent(const Event& event);

    /**
     * @brief Draw all registered views.
     *
     * @param window Reference to the main application window.
     * @param elapsedTime Time elapsed since the last frame, used for animations and updates.
     */
    void draw(IWindow& window, const Time& elapsedTime);

    /**
     * @brief Register a new view with the manager.
     *
     * @param view Unique pointer to the view to register.
     */
    void registerView(UniquePtr<IView> view);

    /**
     * @brief Clear all registered views.
     */
    void clear();

    /**
     * @brief Destroy the manager and release resources.
     */
    void destroy();

  private:
    bool m_initialized;
    Vector<UniquePtr<IView>> m_views;

    /**
     * @copydoc IEventListener::onEvent
     */
    bool onEvent(const Event& event) override;
  };

  template<typename T>
  inline T* editor::EditorViewsManager::getView() const
  {
    if (std::is_base_of<IView, T>::value)
    {
      for (const UniquePtr<IView>& view : m_views)
      {
        if (T* castedView = dynamic_cast<T*>(view.get()))
          return castedView;
      }
    }
    return nullptr;
  }
}

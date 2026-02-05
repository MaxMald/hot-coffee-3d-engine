#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc
{
  class Event;
}

namespace hc::editor
{
  class IView;

  /**
   * @brief Manages editor views and their lifecycle in the editor.
   */
  class EditorViewsManager : public AModule<EditorViewsManager>
  {
  public:
    /**
     * @brief Initialize the editor views manager singleton.
     */
    static void Initialize();

    /**
     * @brief Process an event for all registered views.
     *
     * @param event The event to process.
     *
     * @return True if the event was handled by any view, false otherwise.
     */
    static bool ProcessEvent(const Event& event);

    /**
     * @brief Draw all registered views.
     */
    static void Draw();
    
    /**
     * @brief Retrieve a view of the specified type from the singleton instance.
     * 
     * @tparam T The view type to retrieve. Must derive from IView.
     * 
     * @return Pointer to the view if found, nullptr otherwise.
     */
    template<typename T>
    static T* GetView();

    EditorViewsManager();
    ~EditorViewsManager() override = default;

    /**
     * @brief Initialize the manager and register default views.
     */
    void initialize();

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
     */
    void draw();

    /**
     * @brief Register a new view with the manager.
     * 
     * @param view Unique pointer to the view to register.
     */
    void registerView(UniquePtr<IView> view);

  private:
    bool m_initialized;
    Vector<UniquePtr<IView>> m_views;

    void onPrepare() override;
    void onShutdown() override;
  };

  template<typename T>
  inline T* editor::EditorViewsManager::GetView()
  {
    return EditorViewsManager::Instance().getView<T>();
  }

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

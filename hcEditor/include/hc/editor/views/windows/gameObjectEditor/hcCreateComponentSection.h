#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief UI section for creating components in the editor.
   *
   * Handles user interaction for component instantiation.
   */
  class CreateComponentSection
  {
  public:
    CreateComponentSection();
    ~CreateComponentSection();

    /**
     * @brief Draws the UI section for component creation.
     */
    void draw();

    /**
     * @brief Checks if the user has requested to create a component.
     * 
     * @return true if creation was requested, false otherwise.
     */
    bool wasCreationRequested() const;

    /**
     * @brief Creates a component on the specified GameObject based on the user's
     * selection.
     *
     * @param gameObject The GameObject to which the new component will be added.
     */
    void createComponentFromSelection(GameObject* gameObject);

  private:
    Bool m_userRequestedCreation;
    Int32 m_selectedComponentTypeIndex;

    /**
     * @brief Creates a component of the specified type on the given GameObject.
     * 
     * @tparam ComponentType The type of component to create.
     * 
     * @param gameObject The GameObject to which the component will be added.
     */
    template<typename ComponentType>
    void createComponent(GameObject* gameObject);
  };

  template<typename ComponentType>
  void CreateComponentSection::createComponent(GameObject* gameObject)
  {
    if (gameObject->hasComponent<ComponentType>())
    {
      LogService::Warning("Game Object already has a component of this type.");
      return;
    }

    gameObject->createComponent<ComponentType>();
  }
}

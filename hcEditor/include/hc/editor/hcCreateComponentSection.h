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
     * @brief Creates a component based on the current selection.
     * 
     * @return Unique pointer to the newly created component. Returns nullptr
     * if creation failed or the type is unknown.
     */
    UniquePtr<IComponent> createComponentFromSelection();

  private:
    Bool m_userRequestedCreation;
    Int32 m_selectedComponentTypeIndex;
  };
}

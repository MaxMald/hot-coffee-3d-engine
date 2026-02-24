#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcIComponentDrawer.h"
#include "imgui.h"

namespace hc::editor
{
  /**
   * @brief Abstract base class for component drawers in the editor.
   *
   * This template class provides a common interface and base functionality for
   * all component drawers. It enforces type safety by requiring derived classes
   * to implement the onDrawComponent method for a specific component type.
   *
   * @tparam T The specific component type this drawer handles.
   */
  template<typename T>
  class ABaseComponentDrawer : public IComponentDrawer
  {
  public:
    virtual ~ABaseComponentDrawer() override = default;

    /**
     * @brief Gets the type of the component associated with this drawer.
     *
     * @return The component type.
     */
    componentType::Type getComponentType() const override;

    /**
     * @brief Draws the component in the editor.
     * 
     * Attempts to cast the provided IComponent pointer to the expected type T.
     * If successful, calls onDrawComponent. Otherwise, logs an error.
     * 
     * @param component Pointer to the component to draw.
     */
    void drawComponent(IComponent* component) override;

  protected:
    componentType::Type m_componentType;

    ABaseComponentDrawer(componentType::Type type);

    /**
     * @brief Draws the specific component of type T.
     *
     * This method must be implemented by derived classes to provide custom
     * drawing logic for the component.
     *
     * @param component Pointer to the component of type T.
     */
    virtual void onDrawComponent(T* component) = 0;
  };

  template<typename T>
  ABaseComponentDrawer<T>::ABaseComponentDrawer(componentType::Type type)
    : m_componentType(type)
  {
  }

  template<typename T>
  componentType::Type ABaseComponentDrawer<T>::getComponentType() const
  {
    return m_componentType;
  }

  template<typename T>
  void ABaseComponentDrawer<T>::drawComponent(IComponent* component)
  {
    String componentStr = componentType::ToString(m_componentType);
    if (ImGui::CollapsingHeader(
      String::Format("%s Component", componentStr.c_str()).c_str()
    ))
    {
      T* typedComponent = dynamic_cast<T*>(component);
      if (typedComponent)
      {
        onDrawComponent(typedComponent);
      }
      else
      {
        ImGui::Text(
          "Error: Component ertype mismatch in drawer"
        );
      }
    }
  }
}

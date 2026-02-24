#pragma once

#include "hc/editor/materialDrawer/hcIMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Abstract base class template for material drawer implementations.
   *
   * ABaseMaterialDrawer provides a generic implementation of the IMaterialDrawer
   * interface for materials of type T. It handles type checking and delegates
   * the actual drawing logic to the derived class via the pure virtual onDraw
   * method.
   *
   * @tparam T The material type this drawer handles.
   */
  template<typename T>
  class ABaseMaterialDrawer : public IMaterialDrawer
  {
  public:
    virtual ~ABaseMaterialDrawer() = default;

    /**
     * @brief Draws the material properties in the editor UI.
     *
     * Performs a type check and delegates the drawing logic to onDraw if the
     * material is of the expected type. This method cannot be overridden
     * further.
     *
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material) final;

  protected:
    ABaseMaterialDrawer() = default;

    /**
     * @brief Draws the material-specific properties in the editor UI.
     *
     * This pure virtual method must be implemented by derived classes to handle
     * the actual drawing logic for materials of type T.
     *
     * @param material Pointer to the material of type T.
     */
    virtual void onDraw(T* material) = 0;
  };

  template<typename T>
  void ABaseMaterialDrawer<T>::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    T* typedMaterial = dynamic_cast<T*>(material);
    if (!typedMaterial)
    {
      LogService::Error(
        "Material type mismatch in drawer. Expected: " + String(typeid(T).name())
      );
      return;
    }

    onDraw(typedMaterial);
  }
}

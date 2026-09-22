#pragma once

#include "hc/editor/services/materialDrawer/hcIMaterialDrawer.h"

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
     * @copydoc IMaterialDrawer::drawMaterial
     */
    void drawMaterial(IMaterial* material) final;

    /**
     * @copydoc IMaterialDrawer::drawMeshMaterial
     */
    void drawMeshMaterial(
      IMaterial* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) final;

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

    /**
     * @brief Draws the material properties for a specific material slot in the editor UI.
     *
     * This pure virtual method must be implemented by derived classes to handle the
     * actual drawing logic for materials of type T in a specific material slot.
     *
     * @param material Pointer to the material of type T.
     * @param slotIndex The index of the material slot being drawn.
     * @param projectFileDialogView Reference to the ProjectFileDialogView for file
     * selection.
     */
    virtual void onDrawMeshMaterial(
      T* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) = 0;
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

  template<typename T>
  inline void ABaseMaterialDrawer<T>::drawMeshMaterial(
    IMaterial* material,
    Int32 slotIndex,
    ProjectFileDialogView& projectFileDialogView
  )
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

    onDrawMeshMaterial(typedMaterial, slotIndex, projectFileDialogView);
  }
}

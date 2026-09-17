#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Interface for material drawer implementations in the editor.
   *
   * IMaterialDrawer provides an abstraction for drawing material properties in
   * the editor UI. Implementations should handle rendering material-specific
   * controls and information based on the shading type.
   */
  class IMaterialDrawer
  {
  public:
    virtual ~IMaterialDrawer() = default;

    /**
     * @brief Returns the material type handled by this drawer.
     * 
     * @return The material type as defined in materialType::Type.
     */
    virtual inline materialType::Type getMaterialType() const = 0;

    /**
     * @brief Draws the material properties in the editor UI.
     * 
     * @param material Pointer to the material to be drawn.
     */
    virtual void drawMaterial(IMaterial* material) = 0;

    /**
     * @brief Draws the material properties for a specific material slot in the editor UI.
     *
     * @param material Pointer to the material to be drawn.
     * @param materialSlotIndex The index of the material slot being drawn.
     */
    virtual void drawMeshMaterial(IMaterial* material, Int32 materialSlotIndex) = 0;

  protected:
    IMaterialDrawer() = default;
  };
}

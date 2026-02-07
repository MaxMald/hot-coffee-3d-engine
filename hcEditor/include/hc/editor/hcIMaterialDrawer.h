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
     * @brief Returns the shading type handled by this drawer.
     * 
     * @return The shading type as defined in shadingType::Type.
     */
    virtual shadingType::Type getShadingType() const = 0;

    /**
     * @brief Draws the material properties in the editor UI.
     * 
     * @param material Pointer to the material to be drawn.
     */
    virtual void drawMaterial(IMaterial* material) = 0;

  protected:
    IMaterialDrawer() = default;
  };
}

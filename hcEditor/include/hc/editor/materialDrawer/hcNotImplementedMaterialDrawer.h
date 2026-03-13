#pragma once

#include "hc/editor/materialDrawer/hcIMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Material drawer implementation for unsupported or unimplemented
   * materials.
   */
  class NotImplementedMaterialDrawer : public IMaterialDrawer
  {
  public:
    NotImplementedMaterialDrawer();
    virtual ~NotImplementedMaterialDrawer();

    /**
     * @brief Returns the shading type handled by this drawer.
     * 
     * @return The shading type as defined in shadingType::Type.
     */
    shadingType::Type getShadingType() const override;

    /**
     * @brief Draws a placeholder or warning for unsupported material types.
     *
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material) override;
  };
}

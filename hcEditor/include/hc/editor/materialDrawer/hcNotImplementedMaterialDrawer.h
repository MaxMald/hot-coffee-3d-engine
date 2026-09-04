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
     * @brief Returns the material type handled by this drawer.
     * 
     * @return The material type as defined in materialType::Type.
     */
    materialType::Type getMaterialType() const override;

    /**
     * @brief Draws a placeholder or warning for unsupported material types.
     *
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material) override;
  };
}

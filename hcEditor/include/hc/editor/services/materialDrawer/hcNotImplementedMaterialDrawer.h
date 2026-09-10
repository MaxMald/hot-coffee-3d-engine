#pragma once

#include "hc/editor/services/materialDrawer/hcIMaterialDrawer.h"

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
    inline materialType::Type getMaterialType() const override
    {
      return materialType::Unknown;
    }

    /**
     * @brief Draws a placeholder or warning for unsupported material types.
     *
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material) override;

    /**
     * @copydoc IMaterialDrawer::drawMeshMaterial
     */
    void drawMeshMaterial(IMaterial* material, Int32 materialSlotIndex) override;
  };
}

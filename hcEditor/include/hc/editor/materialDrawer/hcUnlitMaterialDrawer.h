#pragma once

#include "hc/editor/materialDrawer/hcABaseMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Material drawer for unlit materials.
   */
  class UnlitMaterialDrawer : public ABaseMaterialDrawer<UnlitMaterial>
  {
  public:
    UnlitMaterialDrawer() = default;
    virtual ~UnlitMaterialDrawer() = default;

    /**
     * @brief Returns the shading type handled by this drawer.
     * 
     * @return The shading type as defined in shadingType::Type.
     */
    shadingType::Type getShadingType() const override;

  private:

    /**
     * @brief Draws the unlit material properties in the editor UI.
     * 
     * @param material Pointer to the unlit material to be drawn.
     */
    void onDraw(UnlitMaterial* material) override;
  };
}

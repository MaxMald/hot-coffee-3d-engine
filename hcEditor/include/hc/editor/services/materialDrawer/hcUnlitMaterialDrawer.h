#pragma once

#include "hc/editor/services/materialDrawer/hcABaseMaterialDrawer.h"

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
     * @brief Returns the material type handled by this drawer.
     * 
     * @return The material type as defined in materialType::Type.
     */
    inline materialType::Type getMaterialType() const override
    {
      return materialType::Unlit;
    }

  protected:
    void onDraw(UnlitMaterial* material) override;
    void onDrawMeshMaterial(UnlitMaterial* material, Int32 slotIndex) override;
  };
}

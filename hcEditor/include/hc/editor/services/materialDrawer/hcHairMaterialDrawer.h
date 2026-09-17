#pragma once

#include "hc/editor/services/materialDrawer/hcABaseMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Material drawer for Hair materials.
   */
  class HairMaterialDrawer : public ABaseMaterialDrawer<HairMaterial>
  {
  public:
    HairMaterialDrawer() = default;
    virtual  ~HairMaterialDrawer() = default;

    /**
     * @copydoc IMaterialDrawer::getMaterialType
     */
    inline materialType::Type getMaterialType() const override
    {
      return materialType::Hair;
    }

  protected:
    void onDraw(HairMaterial* material) override;
    void onDrawMeshMaterial(HairMaterial* material, Int32 slotIndex) override;
  };
}


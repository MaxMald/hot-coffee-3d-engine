#pragma once

#include "hc/editor/services/materialDrawer/hcABaseMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Material drawer for PBR materials.
   */
  class PBRMaterialDrawer : public ABaseMaterialDrawer<PBRMaterial>
  {
  public:
    PBRMaterialDrawer() = default;
    virtual  ~PBRMaterialDrawer() = default;

    /**
     * @copydoc IMaterialDrawer::getMaterialType
     */
    inline materialType::Type getMaterialType() const override
    {
      return materialType::PBR;
    }

  protected:
    void onDraw(PBRMaterial* material) override;
    void onDrawMeshMaterial(PBRMaterial* material, Int32 slotIndex) override;
  };
}

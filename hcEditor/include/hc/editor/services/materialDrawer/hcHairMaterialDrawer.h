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

    /**
     * @copydoc IMaterialDrawer::drawMaterial
     */
    void onDraw(HairMaterial* material) override;

    /**
     * @copydoc IMaterialDrawer::drawMeshMaterial
     */
    void onDrawMeshMaterial(
      HairMaterial* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) override;
  };
}

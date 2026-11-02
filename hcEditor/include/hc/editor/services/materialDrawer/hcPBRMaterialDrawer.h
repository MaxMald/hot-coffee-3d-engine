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
    PBRMaterialDrawer(ITextureManager& textureManager);
    ~PBRMaterialDrawer() override = default;

    /**
     * @copydoc IMaterialDrawer::getMaterialType
     */
    inline materialType::Type getMaterialType() const override
    {
      return materialType::PBR;
    }

  protected:
    /**
     * @copydoc IMaterialDrawer::drawMaterial
     */
    void onDraw(PBRMaterial* material) override;

    /**
     * @copydoc IMaterialDrawer::drawMeshMaterial
     */
    void onDrawMeshMaterial(
      PBRMaterial* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) override;
  };
}

#pragma once

#include "hc/editor/services/materialDrawer/hcABaseMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Material drawer for Blinn-Phong materials.
   */
  class BlinnPhongMaterialDrawer : public ABaseMaterialDrawer<BlinnPhongMaterial>
  {
  public:
    BlinnPhongMaterialDrawer() = default;
    virtual  ~BlinnPhongMaterialDrawer() = default;

    /**
     * @copydoc IMaterialDrawer::getMaterialType
     */
    inline materialType::Type getMaterialType() const override
    {
      return materialType::BlinnPhong;
    }

  protected:
    void onDraw(BlinnPhongMaterial* material) override;
    void onDrawMeshMaterial(BlinnPhongMaterial* material, Int32 slotIndex) override;
  };
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/materialDrawer/hcNotImplementedMaterialDrawer.h"

namespace hc::editor
{
  /**
   * @brief Manages material drawer instances for different shading types in the
   * editor.
   */
  class MaterialDrawersManager
  {
  public:
    MaterialDrawersManager();
    ~MaterialDrawersManager() = default;

    /**
     * @brief Draws the material using the appropriate drawer for its shading
     * type. If no drawer is registered for the material's shading type, a
     * "not implemented" drawer is used.
     * 
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material);

    /**
     * @brief Registers a new material drawer for a specific shading type.
     * 
     * @param materialDrawer Unique pointer to the material drawer to add.
     */
    void addDrawer(UniquePtr<IMaterialDrawer> materialDrawer);

    /**
     * @brief Removes all registered material drawers.
     */
    void clear();

  private:
    UnorderedMap<materialType::Type, UniquePtr<IMaterialDrawer>> m_drawers;
    NotImplementedMaterialDrawer m_notImplementedDrawer;
  };
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/services/materialDrawer/hcNotImplementedMaterialDrawer.h"
#include "hc/editor/services/hcIEditorService.h"

namespace hc::editor
{
  class ProjectFileDialogView;

  /**
   * @brief Manages material drawer instances for different shading types in the
   * editor.
   */
  class MaterialDrawersManager : public IEditorService
  {
  public:
    MaterialDrawersManager();
    virtual ~MaterialDrawersManager() = default;

    /**
     * @copydoc IEditorService::prepare
     */
    virtual void prepare() override;

    /**
     * @copydoc IEditorService::destroy
     */
    virtual void destroy() override;

    /**
     * @brief Draws the material using the appropriate drawer for its shading type.
     *
     * @note If no drawer is registered for the material's shading type, a "not
     * implemented" drawer is used.
     *
     * @param material Pointer to the material to be drawn.
     */
    void drawMaterial(IMaterial* material);

    /**
     * @brief Draws the material for a specific material slot using the appropriate drawer
     * for its shading type. This drawer allows the user to modify material parameters and
     * load different material textures.
     *
     * @note If no drawer is registered for the material's shading type, a "not
     * implemented" drawer is used.
     *
     * @param material Pointer to the material to be drawn.
     * @param slotIndex The index of the material slot being drawn.
     * @param projectFileDialogView Reference to the ProjectFileDialogView for file
     * selection.
     */
    void drawMeshMaterial(
      IMaterial* material,
      SizeT slotIndex,
      ProjectFileDialogView& projectFileDialogView
    );

    /**
     * @brief Registers a new material drawer for a specific shading type.
     * 
     * @param materialDrawer Unique pointer to the material drawer to add.
     */
    void addDrawer(UniquePtr<IMaterialDrawer> materialDrawer);

  private:
    UnorderedMap<materialType::Type, UniquePtr<IMaterialDrawer>> m_drawers;
    NotImplementedMaterialDrawer m_notImplementedDrawer;
  };
}

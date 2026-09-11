#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ProjectFileDialogView;
  class MaterialDrawersManager;
  class EditorMetadataManager;

  /**
   * @brief Drawer for MeshComponent in the editor.
   */
  class MeshComponentDrawer : public ABaseComponentDrawer<MeshComponent>
  {
  public:
    MeshComponentDrawer(
      IMeshManager& meshManager,
      IAssetManager& assetManager,
      ProjectFileDialogView& projectFileSelector,
      MaterialDrawersManager& materialDrawerManager,
      EditorMetadataManager& editorMetadataManager
    );
    virtual ~MeshComponentDrawer();

  protected:
    IMeshManager& m_meshManager;
    IAssetManager& m_assetManager;
    ProjectFileDialogView& m_projectFileSelector;
    MaterialDrawersManager& m_materialDrawerManager;
    EditorMetadataManager& m_editorMetadataManager;

    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void drawMaterialsInformation(const IMesh& mesh);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
    void onOverrideMaterialClicked(
      const IMesh& mesh,
      const SharedPtr<IMaterial> material,
      SizeT materialIndex
    );
  };
}

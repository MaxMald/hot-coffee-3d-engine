#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ProjectFileDialogView;
  class MaterialDrawersManager;

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
      MaterialDrawersManager& materialDrawerManager
    );
    virtual ~MeshComponentDrawer();

  protected:
    IMeshManager& m_meshManager;
    IAssetManager& m_assetManager;
    ProjectFileDialogView& m_projectFileSelector;
    MaterialDrawersManager& m_materialDrawerManager;

    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void drawMaterialsInformation(const Vector<SharedPtr<IMaterial>>& materials);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
    //void onSaveMeshButtonClicked(MeshComponent* component);
  };
}

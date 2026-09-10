#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ProjectFileDialogView;

  /**
   * @brief Drawer for MeshComponent in the editor.
   */
  class MeshComponentDrawer : public ABaseComponentDrawer<MeshComponent>
  {
  public:
    MeshComponentDrawer(
      IMeshManager& meshManager,
      IAssetManager& assetManager,
      ProjectFileDialogView& projectFileSelector
    );
    virtual ~MeshComponentDrawer();

  protected:
    IMeshManager& m_meshManager;
    IAssetManager& m_assetManager;
    ProjectFileDialogView& m_projectFileSelector;

    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void drawMaterialsInformation(const Vector<SharedPtr<IMaterial>>& materials);
    void drawMaterialInformation(const SharedPtr<IMaterial>& material, Int32 materialSlotIndex);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
  };
}

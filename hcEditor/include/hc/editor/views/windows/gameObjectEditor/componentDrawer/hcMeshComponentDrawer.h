#pragma once

#include "hc/editor/views/windows/gameObjectEditor/componentDrawer/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ProjectFileSelectorView;

  /**
   * @brief Drawer for MeshComponent in the editor.
   */
  class MeshComponentDrawer : public ABaseComponentDrawer<MeshComponent>
  {
  public:
    MeshComponentDrawer(
      IMeshManager& meshManager,
      ProjectFileSelectorView& projectFileSelector
    );
    virtual ~MeshComponentDrawer();

  protected:
    IMeshManager& m_meshManager;
    ProjectFileSelectorView& m_projectFileSelector;

    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
  };
}

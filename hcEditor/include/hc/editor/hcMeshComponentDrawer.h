#pragma once

#include "hc/editor/hcABaseComponentDrawer.h"

namespace hc::editor
{
  class ProjectFileSelector;

  /**
   * @brief Drawer for MeshComponent in the editor.
   */
  class MeshComponentDrawer : public ABaseComponentDrawer<MeshComponent>
  {
  public:
    MeshComponentDrawer(
      IMeshManager& meshManager,
      ProjectFileSelector& projectFileSelector
    );
    virtual ~MeshComponentDrawer();

  protected:
    IMeshManager& m_meshManager;
    ProjectFileSelector& m_projectFileSelector;

    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
  };
}

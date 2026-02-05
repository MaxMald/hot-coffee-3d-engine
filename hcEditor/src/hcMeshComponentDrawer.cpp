#include "hc/editor/hcMeshComponentDrawer.h"
#include "hc/editor/hcProjectFileSelector.h"
#include "imgui.h"

namespace hc::editor
{
  MeshComponentDrawer::MeshComponentDrawer(
    ProjectFileSelector& projectFileSelector
  ) : 
    ABaseComponentDrawer<MeshComponent>(componentType::Mesh),
    m_projectFileSelector(projectFileSelector)
  {
  }

  MeshComponentDrawer::~MeshComponentDrawer()
  {
  }

  void MeshComponentDrawer::onDrawComponent(MeshComponent* component)
  {
    if (!component)
      return;

    drawLoadMeshButton(component);
  }

  void MeshComponentDrawer::drawLoadMeshButton(MeshComponent* component)
  {
    if (ImGui::Button("Load Mesh"))
    {
      m_projectFileSelector.openModelFile(
        [this, component](const Path& selectedPath)
        {
          onMeshFileSelected(component, selectedPath);
        }
      );
    }
  }

  void MeshComponentDrawer::onMeshFileSelected(
    MeshComponent* component,
    const Path& selectedPath
  )
  {
    SharedPtr<IMesh> mesh = 
      HotCoffeeEngine::Instance()
      .getGraphicsManager()
      .getMeshManager()
      .createMeshFromPath(selectedPath);

    component->setMesh(mesh);
  }
}

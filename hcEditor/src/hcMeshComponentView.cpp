#include "hc/editor/hcMeshComponentView.h"
#include "hc/editor/hcProjectFileSelector.h"
#include "imgui.h"

namespace hc::editor
{
  MeshComponentView::MeshComponentView()
    : ABaseComponentView<MeshComponent>(componentType::Mesh)
  {
  }

  MeshComponentView::~MeshComponentView()
  {
  }

  void MeshComponentView::onDrawComponent(MeshComponent* component)
  {
    if (!component)
      return;

    drawLoadMeshButton(component);
  }

  void MeshComponentView::drawLoadMeshButton(MeshComponent* component)
  {
    if (ImGui::Button("Load Mesh"))
    {
      ProjectFileSelector::OpenModelFile(
        [this, component](const Path& selectedPath)
        {
          onMeshFileSelected(component, selectedPath);
        }
      );
    }
  }

  void MeshComponentView::onMeshFileSelected(
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

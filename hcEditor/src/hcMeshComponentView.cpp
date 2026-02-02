#include "hc/editor/hcMeshComponentView.h"

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

    // TODO
  }
}

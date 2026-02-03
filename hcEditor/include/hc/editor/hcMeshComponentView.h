#pragma once

#include "hc/editor/hcABaseComponentView.h"

namespace hc::editor
{
  class MeshComponentView : public ABaseComponentView<MeshComponent>
  {
  public:
    MeshComponentView();
    virtual ~MeshComponentView();

  protected:
    void onDrawComponent(MeshComponent* component) override;
    void drawLoadMeshButton(MeshComponent* component);
    void onMeshFileSelected(MeshComponent* component, const Path& selectedPath);
  };
}

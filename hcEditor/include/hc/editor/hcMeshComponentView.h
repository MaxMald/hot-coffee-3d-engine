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
    virtual void onDrawComponent(MeshComponent* component) override;
  };
}

#pragma once

#include "hc/hcABaseComponent.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  class IMesh;

  class HC_CORE_EXPORT MeshComponent : 
    public ABaseComponent,
    public IDrawable
  {
  public:
    MeshComponent();
    ~MeshComponent() override;

    void draw(const RenderContext& renderContext) override;
    void setMesh(SharedPtr<IMesh> mesh);

  private:
    SharedPtr<IMesh> m_mesh;
  };
}

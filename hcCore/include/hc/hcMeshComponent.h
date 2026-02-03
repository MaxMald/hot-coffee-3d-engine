#pragma once

#include "hc/hcABaseComponent.h"

namespace hc
{
  class IMesh;

  class HC_CORE_EXPORT MeshComponent : public ABaseComponent
  {
  public:
    MeshComponent();
    ~MeshComponent() override;

    void setMesh(SharedPtr<IMesh> mesh);

  private:
    SharedPtr<IMesh> m_mesh;
  };
}

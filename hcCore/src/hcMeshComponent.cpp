#include "hc/hcMeshComponent.h"
#include "hc/hcIMesh.h"

namespace hc
{
  MeshComponent::MeshComponent() :
    ABaseComponent(componentType::Mesh)
  {
  }

  MeshComponent::~MeshComponent()
  {
  }

  void MeshComponent::setMesh(SharedPtr<IMesh> mesh)
  {
    m_mesh = mesh;
  }
}

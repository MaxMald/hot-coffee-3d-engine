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

  void MeshComponent::draw(const RenderContext& renderContext)
  {
    if (m_mesh)
      m_mesh->draw(renderContext);
  }

  void MeshComponent::setMesh(SharedPtr<IMesh> mesh)
  {
    m_mesh = mesh;
  }
}

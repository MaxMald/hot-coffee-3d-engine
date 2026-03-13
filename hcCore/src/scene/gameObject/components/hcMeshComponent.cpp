#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/graphics/resource/mesh/hcIMesh.h"

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
    if (!m_mesh)
      return;

    m_mesh->draw(renderContext);
  }

  void MeshComponent::setMesh(SharedPtr<IMesh> mesh)
  {
    m_mesh = mesh;
  }

  SharedPtr<IMesh> MeshComponent::getMesh() const
  {
    return m_mesh;
  }
}

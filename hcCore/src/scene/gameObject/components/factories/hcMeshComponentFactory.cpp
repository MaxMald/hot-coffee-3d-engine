#include "hc/scene/gameObject/components/factories/hcMeshComponentFactory.h"

namespace hc
{
  MeshComponentFactory::MeshComponentFactory(IMeshManager& meshManager)
    : m_meshManager(meshManager)
  {
  }

  UniquePtr<MeshComponent> MeshComponentFactory::create() const
  {
    return MakeUnique<MeshComponent>(m_meshManager);
  }
}

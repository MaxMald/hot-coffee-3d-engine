#include "hc/scene/gameObject/components/factories/hcMeshComponentFactory.h"
#include "hc/assets/hcIAssetManager.h"

namespace hc
{
  MeshComponentFactory::MeshComponentFactory(
    IMeshManager& meshManager,
    IAssetManager& assetManager) :
    m_meshManager(meshManager),
    m_assetManager(assetManager)
  {
  }

  UniquePtr<MeshComponent> MeshComponentFactory::create() const
  {
    return MakeUnique<MeshComponent>(m_meshManager, m_assetManager);
  }
}

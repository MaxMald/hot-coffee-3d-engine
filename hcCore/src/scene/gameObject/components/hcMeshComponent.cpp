#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/graphics/resource/mesh/hcIMesh.h"
#include "hc/graphics/resource/mesh/hcIMeshManager.h"
#include "hc/assets/model/hcModel.h"

namespace hc
{
  MeshComponent::MeshComponent(IMeshManager& meshManager) :
    ABaseComponent(componentType::Mesh),
    m_mesh(nullptr),
    m_meshManager(meshManager)
  {
  }

  MeshComponent::~MeshComponent()
  {
  }

  void MeshComponent::serialize(BinaryWriter& writer) const
  {
    bool hasMesh = (m_mesh != nullptr && m_mesh->getModel() != nullptr);
    writer.writeBool(hasMesh);

    if (hasMesh)
      writer.writePath(m_mesh->getModel()->getPath());
  }

  void MeshComponent::deserialize(BinaryReader& reader)
  {
    bool hasMesh = reader.readBool();
    if (!hasMesh)
    {
      m_mesh = nullptr;
      return;
    }

    Path modelPath = reader.readPath();
    m_mesh = m_meshManager.createMeshFromPath(modelPath);
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

#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/graphics/resource/mesh/hcIMesh.h"
#include "hc/graphics/resource/mesh/hcIMeshManager.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/model/hcModel.h"

namespace hc
{
  static constexpr UInt16 MESH_COMPONENT_VERSION = 1;

  MeshComponent::MeshComponent(
    IMeshManager& meshManager,
    IAssetManager& assetManager
  ) :
    ABaseComponent(componentType::Mesh),
    m_sourcePath(),
    m_mesh(nullptr),
    m_meshManager(meshManager),
    m_assetManager(assetManager)
  {}

  MeshComponent::~MeshComponent()
  {}

  void MeshComponent::draw(
    const RenderContext& renderContext,
    Vector<DrawCommand>& drawCommands
  ) const
  {
    if (!m_mesh)
      return;

    m_mesh->draw(renderContext, drawCommands);
  }

  void MeshComponent::setMesh(SharedPtr<IMesh> mesh, const Path& sourcePath)
  {
    m_mesh = mesh;
    m_sourcePath = sourcePath;
  }

  void MeshComponent::clear()
  {
    m_mesh.reset();
    m_sourcePath.clear();
  }

  void MeshComponent::onSerialize(io::BinaryWriter& writer) const
  {
    bool hasMesh = (m_mesh != nullptr && !m_mesh->getSourcePath().empty());
    writer.writeBool(hasMesh);

    if (!hasMesh)
      return;

    writer.writePath(m_sourcePath);
  }

  void MeshComponent::onDeserialize(io::BinaryReader& reader)
  {
    clear();

    bool hasMesh = reader.readBool();
    if (!hasMesh)
      return;

    m_sourcePath = reader.readPath();
    if (m_sourcePath.isAbsolute())
    {
      m_mesh = m_meshManager.createMeshFromPath(m_sourcePath);
      return;
    }
      
    if (!m_assetManager.hasRootPath())
      throw RuntimeErrorException(
        "Cannot load mesh during deserialization: asset manager does not have a root path set for relative paths"
      );

    Path absoluteModelPath = m_sourcePath.toAbsolute(m_assetManager.getRootPath());
    m_mesh = m_meshManager.createMeshFromPath(absoluteModelPath);
  }

  UInt16 MeshComponent::getDerivedVersion() const
  {
    return MESH_COMPONENT_VERSION;
  }
}

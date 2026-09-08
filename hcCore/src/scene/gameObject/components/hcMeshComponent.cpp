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
    m_mesh(nullptr),
    m_meshManager(meshManager),
    m_assetManager(assetManager)
  {
  }

  MeshComponent::~MeshComponent()
  {
  }

  void MeshComponent::draw(
    const RenderContext& renderContext,
    Vector<DrawCommand>& drawCommands
  ) const
  {
    if (!m_mesh)
      return;

    m_mesh->draw(renderContext, drawCommands);
  }

  void MeshComponent::setMesh(SharedPtr<IMesh> mesh)
  {
    m_mesh = mesh;
  }

  SharedPtr<IMesh> MeshComponent::getMesh() const
  {
    return m_mesh;
  }

  void MeshComponent::onSerialize(io::BinaryWriter& writer) const
  {
    // TODO
    //
    // Currently we only serialize the mesh by its source path, which means we can only
    // reconstruct the mesh during deserialization if it was originally created from a
    // model file. This is a limitation that should be addressed in the future by implementing a more
    // robust serialization mechanism that can handle meshes created procedurally or from
    // other sources.

    bool hasMesh = (m_mesh != nullptr && !m_mesh->getSourcePath().empty());
    writer.writeBool(hasMesh);

    if (!hasMesh)
      return;

    Path modelPath = m_mesh->getSourcePath();
    Path pathToSerialize = modelPath;

    if (m_assetManager.hasRootPath())
      pathToSerialize = modelPath.toRelative(m_assetManager.getRootPath());

    writer.writePath(pathToSerialize);
  }

  void MeshComponent::onDeserialize(io::BinaryReader& reader)
  {
    bool hasMesh = reader.readBool();
    if (!hasMesh)
    {
      m_mesh = nullptr;
      return;
    }

    Path modelPath = reader.readPath();

    if (modelPath.isRelative())
    {
      if (!m_assetManager.hasRootPath())
      {
        throw RuntimeErrorException(
          "Cannot load mesh during deserialization: asset manager does not have a root path set for relative paths"
        );
      }

      Path absoluteModelPath = modelPath.toAbsolute(m_assetManager.getRootPath());
      m_mesh = m_meshManager.createMeshFromPath(absoluteModelPath);
    }
    else
    {
      m_mesh = m_meshManager.createMeshFromPath(modelPath);
    }
  }

  UInt16 MeshComponent::getDerivedVersion() const
  {
    return MESH_COMPONENT_VERSION;
  }
}

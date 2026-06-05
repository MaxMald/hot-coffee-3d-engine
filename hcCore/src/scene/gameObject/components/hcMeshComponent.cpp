#include "hc/scene/gameObject/components/hcMeshComponent.h"
#include "hc/graphics/resource/mesh/hcIMesh.h"
#include "hc/graphics/resource/mesh/hcIMeshManager.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/hcAssetPath.h"
#include "hc/assets/model/hcModel.h"

namespace hc
{
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

  void MeshComponent::serialize(BinaryWriter& writer) const
  {
    ABaseComponent::serialize(writer);

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
    String modelPathStr = modelPath.generic_string();

    if (m_assetManager.hasRootPath())
      modelPathStr = AssetPath::ToRelative(modelPath, m_assetManager.getRootPath());

    writer.writeString(modelPathStr);
  }

  void MeshComponent::deserialize(BinaryReader& reader)
  {
    ABaseComponent::deserialize(reader);

    bool hasMesh = reader.readBool();
    if (!hasMesh)
    {
      m_mesh = nullptr;
      return;
    }

    String modelPathStr = reader.readString();

    if (AssetPath::IsRelative(modelPathStr))
    {
      if (!m_assetManager.hasRootPath())
      {
        throw RuntimeErrorException(
          "Cannot load mesh during deserialization: asset manager does not have a root path set for relative paths"
        );
      }

      Path modelPath = AssetPath::ToAbsolute(modelPathStr, m_assetManager.getRootPath());
      m_mesh = m_meshManager.createMeshFromPath(modelPath);
    }
    else
    {
      m_mesh = m_meshManager.createMeshFromPath(modelPathStr.c_str());
    }
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

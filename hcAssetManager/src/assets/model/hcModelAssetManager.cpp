#include "hc/assets/model/hcModelAssetManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "hc/assets/model/hcAssimpMaterialDescriptorParser.h"

namespace hc
{
    ModelAssetManager::ModelAssetManager(
      IMaterialDescriptorAssetManager& materialDescriptorAssetManager
    ) :
      m_loadedModels(),
      m_primitiveModels(),
      m_primitiveModelsFactory(materialDescriptorAssetManager)
    {
    }

    SharedPtr<Model> ModelAssetManager::load(const Path& path)
  {
    if (isLoaded(path))
      return get(path);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
      path.string().c_str(),
      aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs
    );

    if (!scene || !scene->mRootNode)
      return nullptr;

    UInt32 totalVertices = calculateTotalVertices(scene);
    UInt32 totalIndices = calculateTotalIndices(scene);
    Buffer<Vertex> vertices(totalVertices);
    BufferUInt32 indices(totalIndices);

    Vector<ModelSubMesh> subMeshes;

    UInt32 vertexOffset = 0;
    UInt32 indexOffset = 0;

    for (UInt32 i = 0; i < scene->mNumMeshes; ++i)
    {
      aiMesh* mesh = scene->mMeshes[i];

      ModelSubMesh subMesh;
      subMesh.firstVertexIndex = vertexOffset;
      subMesh.firstIndexIndex = indexOffset;
      subMesh.vertexCount = mesh->mNumVertices;
      subMesh.materialIndex = mesh->mMaterialIndex;

      for (UInt32 v = 0; v < mesh->mNumVertices; ++v)
      {
        SizeT globalIndex = static_cast<SizeT>(vertexOffset + v);
        copyAssimpVertex(mesh, v, vertices[globalIndex]);
      }

      for (UInt32 f = 0; f < mesh->mNumFaces; ++f)
      {
        aiFace& face = mesh->mFaces[f];
        for (UInt32 idx = 0; idx < face.mNumIndices; ++idx)
        {
          SizeT globalIndex = static_cast<SizeT>(indexOffset++);
          indices[globalIndex] = vertexOffset + face.mIndices[idx];
        }
      }

      subMesh.indexCount = indexOffset - subMesh.firstIndexIndex;
      vertexOffset += mesh->mNumVertices;

      subMeshes.push_back(subMesh);
    }

    Vector<SharedPtr<AMaterialDescriptor>> materialDescriptors;
    for (UInt32 i = 0; i < scene->mNumMaterials; ++i)
    {
      materialDescriptors.push_back(
        AssimpMaterialDescriptorParser::Parse(
          path.parent_path(),
          scene->mMaterials[i]
        )
      );
    }

    SharedPtr<Model> model = MakeShared<Model>(
      path,
      std::move(vertices),
      std::move(indices),
      subMeshes,
      materialDescriptors
    );

    m_loadedModels[path] = model;
    return model;
  }

  SharedPtr<Model> ModelAssetManager::get(const Path& path) const
  {
    auto it = m_loadedModels.find(path);
    if (it != m_loadedModels.end())
      return it->second;

    throw RuntimeErrorException(
      "Model asset not found: " + path.string()
    );
  }

  bool ModelAssetManager::isLoaded(const Path& path) const
  {
    return m_loadedModels.find(path) != m_loadedModels.end();
  }

  void ModelAssetManager::clear()
  {
    m_loadedModels.clear();
    m_primitiveModels.clear();
  }

  SharedPtr<Model> ModelAssetManager::getPrimitive(
    primitiveModelType::Type primitiveType
  )
  {
    if (hasCachedPrimitive(primitiveType))
      return m_primitiveModels[primitiveType];

    m_primitiveModels[primitiveType] = m_primitiveModelsFactory
      .createFromPrimitiveType(primitiveType);

    return m_primitiveModels[primitiveType];
  }

  UInt32 ModelAssetManager::calculateTotalVertices(const aiScene* scene)
  {
    UInt32 totalVertices = 0;
    for (UInt32 i = 0; i < scene->mNumMeshes; ++i)
    {
      aiMesh* mesh = scene->mMeshes[i];
      totalVertices += mesh->mNumVertices;
    }
    return totalVertices;
  }

  UInt32 ModelAssetManager::calculateTotalIndices(const aiScene* scene)
  {
    UInt32 totalIndices = 0;
    for (UInt32 i = 0; i < scene->mNumMeshes; ++i)
    {
      aiMesh* mesh = scene->mMeshes[i];
      for (UInt32 j = 0; j < mesh->mNumFaces; ++j)
      {
        aiFace& face = mesh->mFaces[j];
        totalIndices += face.mNumIndices;
      }
    }
    return totalIndices;
  }

  void ModelAssetManager::copyAssimpVertex(
    const aiMesh* mesh,
    UInt32 assimpVertexIndex,
    Vertex& outVertex
  )
  {
    outVertex.position = Vector3(
      mesh->mVertices[assimpVertexIndex].x,
      mesh->mVertices[assimpVertexIndex].y,
      mesh->mVertices[assimpVertexIndex].z
    );

    if (mesh->mNormals)
    {
      outVertex.normal = Vector3(
        mesh->mNormals[assimpVertexIndex].x,
        mesh->mNormals[assimpVertexIndex].y,
        mesh->mNormals[assimpVertexIndex].z
      );
    }

    if (mesh->mTextureCoords[0])
    {
      outVertex.texCoord = Vector2(
        mesh->mTextureCoords[0][assimpVertexIndex].x,
        mesh->mTextureCoords[0][assimpVertexIndex].y
      );
    }

    if (mesh->mTangents)
    {
      outVertex.tangent = Vector3(
        mesh->mTangents[assimpVertexIndex].x,
        mesh->mTangents[assimpVertexIndex].y,
        mesh->mTangents[assimpVertexIndex].z
      );
    }

    if (mesh->mColors[0])
    {
      outVertex.color = Color(
        mesh->mColors[0][assimpVertexIndex].r,
        mesh->mColors[0][assimpVertexIndex].g,
        mesh->mColors[0][assimpVertexIndex].b,
        mesh->mColors[0][assimpVertexIndex].a
      );
    }
  }

  bool ModelAssetManager::hasCachedPrimitive(
    primitiveModelType::Type primitiveType
  ) const
  {
    return m_primitiveModels.find(primitiveType) != m_primitiveModels.end();
  }
}

#include "hc/hcModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace hc
{
  ModelLoader::ModelLoader()
    : IModelLoader()
  {
  }

  ModelLoader::~ModelLoader()
  {
  }

  SharedPtr<Model> ModelLoader::load(const Path& filePath)
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
      filePath.string().c_str(),
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

    Vector<SharedPtr<MaterialDescriptor>> materialDescriptors;
    for (UInt32 i = 0; i < scene->mNumMaterials; ++i)
    {
      // TODO
    }

    return MakeShared<Model>(
      std::move(vertices),
      std::move(indices),
      std::move(subMeshes),
      std::move(materialDescriptors)
    );
  }

  UInt32 ModelLoader::calculateTotalVertices(const aiScene* scene)
  {
    UInt32 totalVertices = 0;
    for (UInt32 i = 0; i < scene->mNumMeshes; ++i)
    {
      aiMesh* mesh = scene->mMeshes[i];
      totalVertices += mesh->mNumVertices;
    }
    return totalVertices;
  }

  UInt32 ModelLoader::calculateTotalIndices(const aiScene* scene)
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

  void ModelLoader::copyAssimpVertex(
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

}

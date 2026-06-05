#include "hc/assets/model/hcPrimitiveModelsFactory.h"
#include "hc/assets/model/hcModel.h"
#include "hc/assets/model/hcPrimitiveModelPathUtilities.h"
#include "hc/assets/materialDescriptor/hcIMaterialDescriptorAssetManager.h"

namespace hc
{
  PrimitiveModelsFactory::PrimitiveModelsFactory(
    IMaterialDescriptorAssetManager& materialDescriptorAssetManager
  ) :
    m_materialDescriptorAssetManager(materialDescriptorAssetManager)
  {
  }

  SharedPtr<Model> PrimitiveModelsFactory::createFromPrimitiveType(
    primitiveModelType::Type primitiveType
  )
  {
    switch (primitiveType)
    {
    case primitiveModelType::Cube:
      return createCube();
    case primitiveModelType::Cone:
      return createCone();
    case primitiveModelType::Sphere:
      return createSphere();
    case primitiveModelType::Cylinder:
      return createCylinder();
    case primitiveModelType::Plane:
      return createPlane();
    case primitiveModelType::Pyramid:
      return createPyramid();
    case primitiveModelType::ConeNoBase:
      return createConeNoBase();
    default:
      throw InvalidArgumentException(
        "Unsupported primitive model type: " +
        primitiveModelType::toString(primitiveType)
      );
    }
  }

  SharedPtr<Model> PrimitiveModelsFactory::createCube()
  {
    Buffer<Vertex> vertices(8);
    BufferUInt32 indices(36);

    vertices[0].position = Vector3f(-0.5f, -0.5f, -0.5f);
    vertices[0].normal = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[0].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[0].texCoord = Vector2f(0.0f, 0.0f);

    vertices[1].position = Vector3f(0.5f, -0.5f, -0.5f);
    vertices[1].normal = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[1].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[1].texCoord = Vector2f(1.0f, 0.0f);

    vertices[2].position = Vector3f(0.5f, 0.5f, -0.5f);
    vertices[2].normal = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[2].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[2].texCoord = Vector2f(1.0f, 1.0f);

    vertices[3].position = Vector3f(-0.5f, 0.5f, -0.5f);
    vertices[3].normal = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[3].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[3].texCoord = Vector2f(0.0f, 1.0f);

    vertices[4].position = Vector3f(-0.5f, -0.5f, 0.5f);
    vertices[4].normal = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[4].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[4].texCoord = Vector2f(0.0f, 0.0f);

    vertices[5].position = Vector3f(0.5f, -0.5f, 0.5f);
    vertices[5].normal = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[5].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[5].texCoord = Vector2f(1.0f, 0.0f);

    vertices[6].position = Vector3f(0.5f, 0.5f, 0.5f);
    vertices[6].normal = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[6].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[6].texCoord = Vector2f(1.0f, 1.0f);

    vertices[7].position = Vector3f(-0.5f, 0.5f, 0.5f);
    vertices[7].normal = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[7].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[7].texCoord = Vector2f(0.0f, 1.0f);

    UInt32 indexData[36] = {
      0, 2, 1, 0, 3, 2,
      4, 5, 6, 4, 6, 7,
      0, 4, 7, 0, 7, 3,
      1, 2, 6, 1, 6, 5,
      0, 1, 5, 0, 5, 4,
      3, 7, 6, 3, 6, 2
    };

    for (UInt32 i = 0; i < 36; ++i)
    {
      indices[i] = indexData[i];
    }

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Cube),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createCone()
  {
    const UInt32 segments = 32;
    const float radius = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height * 0.5f;

    const UInt32 vertexCount = (segments + 1) * 2;
    const UInt32 indexCount = segments * 6;

    Buffer<Vertex> vertices(vertexCount);
    BufferUInt32 indices(indexCount);

    UInt32 vertexIndex = 0;
    UInt32 indexIndex = 0;

    const float sideNormalY = radius / Math::Sqrt(radius * radius + height * height);
    const float sideNormalXZ = height / Math::Sqrt(radius * radius + height * height);

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, -halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(x * sideNormalXZ / radius, sideNormalY, z * sideNormalXZ / radius);
      vertices[vertexIndex].tangent = Vector3f(-Math::Sin(angle), 0.0f, Math::Cos(angle));
      vertices[vertexIndex].texCoord = Vector2f(static_cast<float>(i) / static_cast<float>(segments), 1.0f);
      vertexIndex++;
    }

    vertices[vertexIndex].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[vertexIndex].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[vertexIndex].texCoord = Vector2f(0.5f, 0.0f);
    UInt32 apexIndex = vertexIndex;
    vertexIndex++;

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = i;
      indices[indexIndex++] = apexIndex;
      indices[indexIndex++] = next;
    }

    vertices[vertexIndex].position = Vector3f(0.0f, -halfHeight, 0.0f);
    vertices[vertexIndex].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[vertexIndex].texCoord = Vector2f(0.5f, 0.5f);
    UInt32 baseCenterIndex = vertexIndex;
    vertexIndex++;

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, -halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(0.0f, -1.0f, 0.0f);
      vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
      float u = (Math::Cos(angle) + 1.0f) * 0.5f;
      float v = (Math::Sin(angle) + 1.0f) * 0.5f;
      vertices[vertexIndex].texCoord = Vector2f(u, v);
      vertexIndex++;
    }

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = baseCenterIndex;
      indices[indexIndex++] = baseCenterIndex + 1 + next;
      indices[indexIndex++] = baseCenterIndex + 1 + i;
    }

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Cone),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createSphere()
  {
    const UInt32 latitudeSegments = 32;
    const UInt32 longitudeSegments = 32;
    const float radius = 0.5f;

    const UInt32 vertexCount = (latitudeSegments + 1) * (longitudeSegments + 1);
    const UInt32 indexCount = latitudeSegments * longitudeSegments * 6;

    Buffer<Vertex> vertices(vertexCount);
    BufferUInt32 indices(indexCount);

    UInt32 vertexIndex = 0;
    UInt32 indexIndex = 0;

    for (UInt32 lat = 0; lat <= latitudeSegments; ++lat)
    {
      float theta = static_cast<float>(lat)
        * Math::Pi
        / static_cast<float>(latitudeSegments);

      float sinTheta = Math::Sin(theta);
      float cosTheta = Math::Cos(theta);

      for (UInt32 lon = 0; lon <= longitudeSegments; ++lon)
      {
        float phi = static_cast<float>(lon)
          * 2.0f
          * Math::Pi
          / static_cast<float>(longitudeSegments);

        float sinPhi = Math::Sin(phi);
        float cosPhi = Math::Cos(phi);

        float x = cosPhi * sinTheta;
        float y = cosTheta;
        float z = sinPhi * sinTheta;

        vertices[vertexIndex].position = Vector3f(x * radius, y * radius, z * radius);
        vertices[vertexIndex].normal = Vector3f(x, y, z);
        vertices[vertexIndex].tangent = Vector3f(-sinPhi, 0.0f, cosPhi);
        vertices[vertexIndex].texCoord = Vector2f(
          static_cast<float>(lon) / static_cast<float>(longitudeSegments),
          static_cast<float>(lat) / static_cast<float>(latitudeSegments)
        );
        vertexIndex++;
      }
    }

    for (UInt32 lat = 0; lat < latitudeSegments; ++lat)
    {
      for (UInt32 lon = 0; lon < longitudeSegments; ++lon)
      {
        UInt32 first = lat * (longitudeSegments + 1) + lon;
        UInt32 second = first + longitudeSegments + 1;

        indices[indexIndex++] = first;
        indices[indexIndex++] = second;
        indices[indexIndex++] = first + 1;

        indices[indexIndex++] = second;
        indices[indexIndex++] = second + 1;
        indices[indexIndex++] = first + 1;
      }
    }

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Sphere),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createCylinder()
  {
    const UInt32 segments = 32;
    const float radius = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height * 0.5f;

    const UInt32 vertexCount = segments * 2 + segments * 2 + 2;
    const UInt32 indexCount = segments * 6 + segments * 6;

    Buffer<Vertex> vertices(vertexCount);
    BufferUInt32 indices(indexCount);

    UInt32 vertexIndex = 0;
    UInt32 indexIndex = 0;

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, -halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(x / radius, 0.0f, z / radius);
      vertices[vertexIndex].tangent = Vector3f(-Math::Sin(angle), 0.0f, Math::Cos(angle));
      vertices[vertexIndex].texCoord = Vector2f(static_cast<float>(i) / static_cast<float>(segments), 1.0f);
      vertexIndex++;
    }

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(x / radius, 0.0f, z / radius);
      vertices[vertexIndex].tangent = Vector3f(-Math::Sin(angle), 0.0f, Math::Cos(angle));
      vertices[vertexIndex].texCoord = Vector2f(static_cast<float>(i) / static_cast<float>(segments), 0.0f);
      vertexIndex++;
    }

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = i;
      indices[indexIndex++] = segments + i;
      indices[indexIndex++] = next;

      indices[indexIndex++] = next;
      indices[indexIndex++] = segments + i;
      indices[indexIndex++] = segments + next;
    }

    UInt32 bottomCapStart = vertexIndex;
    vertices[vertexIndex].position = Vector3f(0.0f, -halfHeight, 0.0f);
    vertices[vertexIndex].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[vertexIndex].texCoord = Vector2f(0.5f, 0.5f);
    vertexIndex++;

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, -halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(0.0f, -1.0f, 0.0f);
      vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
      float u = (Math::Cos(angle) + 1.0f) * 0.5f;
      float v = (Math::Sin(angle) + 1.0f) * 0.5f;
      vertices[vertexIndex].texCoord = Vector2f(u, v);
      vertexIndex++;
    }

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = bottomCapStart;
      indices[indexIndex++] = bottomCapStart + 1 + next;
      indices[indexIndex++] = bottomCapStart + 1 + i;
    }

    UInt32 topCapStart = vertexIndex;
    vertices[vertexIndex].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[vertexIndex].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[vertexIndex].texCoord = Vector2f(0.5f, 0.5f);
    vertexIndex++;

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(0.0f, 1.0f, 0.0f);
      vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
      float u = (Math::Cos(angle) + 1.0f) * 0.5f;
      float v = (Math::Sin(angle) + 1.0f) * 0.5f;
      vertices[vertexIndex].texCoord = Vector2f(u, v);
      vertexIndex++;
    }

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = topCapStart;
      indices[indexIndex++] = topCapStart + 1 + i;
      indices[indexIndex++] = topCapStart + 1 + next;
    }

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Cylinder),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createPlane()
  {
    Buffer<Vertex> vertices(4);
    BufferUInt32 indices(6);

    vertices[0].position = Vector3f(-0.5f, 0.0f, -0.5f);
    vertices[0].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[0].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[0].texCoord = Vector2f(0.0f, 0.0f);

    vertices[1].position = Vector3f(0.5f, 0.0f, -0.5f);
    vertices[1].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[1].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[1].texCoord = Vector2f(1.0f, 0.0f);

    vertices[2].position = Vector3f(0.5f, 0.0f, 0.5f);
    vertices[2].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[2].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[2].texCoord = Vector2f(1.0f, 1.0f);

    vertices[3].position = Vector3f(-0.5f, 0.0f, 0.5f);
    vertices[3].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[3].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[3].texCoord = Vector2f(0.0f, 1.0f);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Plane),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createPyramid()
  {
    Buffer<Vertex> vertices(16);
    BufferUInt32 indices(18);

    const float halfSize = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height * 0.5f;

    const float sideNormalY = halfSize / Math::Sqrt(halfSize * halfSize + height * height);
    const float sideNormalXZ = height / Math::Sqrt(halfSize * halfSize + height * height);

    vertices[0].position = Vector3f(-halfSize, -halfHeight, -halfSize);
    vertices[0].normal = Vector3f(0.0f, 0.0f, -sideNormalXZ);
    vertices[0].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[0].texCoord = Vector2f(0.0f, 1.0f);

    vertices[1].position = Vector3f(halfSize, -halfHeight, -halfSize);
    vertices[1].normal = Vector3f(0.0f, 0.0f, -sideNormalXZ);
    vertices[1].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[1].texCoord = Vector2f(1.0f, 1.0f);

    vertices[2].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[2].normal = Vector3f(0.0f, 0.0f, -sideNormalXZ);
    vertices[2].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[2].texCoord = Vector2f(0.5f, 0.0f);

    vertices[3].position = Vector3f(halfSize, -halfHeight, -halfSize);
    vertices[3].normal = Vector3f(sideNormalXZ, 0.0f, 0.0f);
    vertices[3].tangent = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[3].texCoord = Vector2f(0.0f, 1.0f);

    vertices[4].position = Vector3f(halfSize, -halfHeight, halfSize);
    vertices[4].normal = Vector3f(sideNormalXZ, 0.0f, 0.0f);
    vertices[4].tangent = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[4].texCoord = Vector2f(1.0f, 1.0f);

    vertices[5].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[5].normal = Vector3f(sideNormalXZ, 0.0f, 0.0f);
    vertices[5].tangent = Vector3f(0.0f, 0.0f, 1.0f);
    vertices[5].texCoord = Vector2f(0.5f, 0.0f);

    vertices[6].position = Vector3f(halfSize, -halfHeight, halfSize);
    vertices[6].normal = Vector3f(0.0f, 0.0f, sideNormalXZ);
    vertices[6].tangent = Vector3f(-1.0f, 0.0f, 0.0f);
    vertices[6].texCoord = Vector2f(0.0f, 1.0f);

    vertices[7].position = Vector3f(-halfSize, -halfHeight, halfSize);
    vertices[7].normal = Vector3f(0.0f, 0.0f, sideNormalXZ);
    vertices[7].tangent = Vector3f(-1.0f, 0.0f, 0.0f);
    vertices[7].texCoord = Vector2f(1.0f, 1.0f);

    vertices[8].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[8].normal = Vector3f(0.0f, 0.0f, sideNormalXZ);
    vertices[8].tangent = Vector3f(-1.0f, 0.0f, 0.0f);
    vertices[8].texCoord = Vector2f(0.5f, 0.0f);

    vertices[9].position = Vector3f(-halfSize, -halfHeight, halfSize);
    vertices[9].normal = Vector3f(-sideNormalXZ, sideNormalY, 0.0f);
    vertices[9].tangent = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[9].texCoord = Vector2f(0.0f, 1.0f);

    vertices[10].position = Vector3f(-halfSize, -halfHeight, -halfSize);
    vertices[10].normal = Vector3f(-sideNormalXZ, sideNormalY, 0.0f);
    vertices[10].tangent = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[10].texCoord = Vector2f(1.0f, 1.0f);

    vertices[11].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[11].normal = Vector3f(-sideNormalXZ, sideNormalY, 0.0f);
    vertices[11].tangent = Vector3f(0.0f, 0.0f, -1.0f);
    vertices[11].texCoord = Vector2f(0.5f, 0.0f);

    vertices[12].position = Vector3f(-halfSize, -halfHeight, -halfSize);
    vertices[12].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[12].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[12].texCoord = Vector2f(0.0f, 0.0f);

    vertices[13].position = Vector3f(halfSize, -halfHeight, -halfSize);
    vertices[13].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[13].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[13].texCoord = Vector2f(1.0f, 0.0f);

    vertices[14].position = Vector3f(halfSize, -halfHeight, halfSize);
    vertices[14].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[14].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[14].texCoord = Vector2f(1.0f, 1.0f);

    vertices[15].position = Vector3f(-halfSize, -halfHeight, halfSize);
    vertices[15].normal = Vector3f(0.0f, -1.0f, 0.0f);
    vertices[15].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[15].texCoord = Vector2f(0.0f, 1.0f);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;
    indices[4] = 4;
    indices[5] = 5;
    indices[6] = 6;
    indices[7] = 7;
    indices[8] = 8;
    indices[9] = 9;
    indices[10] = 10;
    indices[11] = 11;
    indices[12] = 12;
    indices[13] = 13;
    indices[14] = 14;
    indices[15] = 12;
    indices[16] = 14;
    indices[17] = 15;

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(primitiveModelType::Pyramid),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }

  SharedPtr<Model> PrimitiveModelsFactory::createConeNoBase()
  {
    const UInt32 segments = 32;
    const float radius = 0.5f;
    const float height = 1.0f;
    const float halfHeight = height * 0.5f;

    const UInt32 vertexCount = segments + 1; // ring + apex
    const UInt32 indexCount = segments * 3;  // side triangles only

    Buffer<Vertex> vertices(vertexCount);
    BufferUInt32 indices(indexCount);

    UInt32 vertexIndex = 0;
    UInt32 indexIndex = 0;

    const float sideNormalY = radius / Math::Sqrt(radius * radius + height * height);
    const float sideNormalXZ = height / Math::Sqrt(radius * radius + height * height);

    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = static_cast<float>(i)
        * 2.0f
        * Math::Pi
        / static_cast<float>(segments);

      float x = Math::Cos(angle) * radius;
      float z = Math::Sin(angle) * radius;

      vertices[vertexIndex].position = Vector3f(x, -halfHeight, z);
      vertices[vertexIndex].normal = Vector3f(
        x * sideNormalXZ / radius,
        sideNormalY,
        z * sideNormalXZ / radius
      );
      vertices[vertexIndex].tangent = Vector3f(-Math::Sin(angle), 0.0f, Math::Cos(angle));
      vertices[vertexIndex].texCoord = Vector2f(
        static_cast<float>(i) / static_cast<float>(segments),
        1.0f
      );
      vertexIndex++;
    }

    vertices[vertexIndex].position = Vector3f(0.0f, halfHeight, 0.0f);
    vertices[vertexIndex].normal = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[vertexIndex].tangent = Vector3f(1.0f, 0.0f, 0.0f);
    vertices[vertexIndex].texCoord = Vector2f(0.5f, 0.0f);
    UInt32 apexIndex = vertexIndex;

    for (UInt32 i = 0; i < segments; ++i)
    {
      UInt32 next = (i + 1) % segments;
      indices[indexIndex++] = i;
      indices[indexIndex++] = apexIndex;
      indices[indexIndex++] = next;
    }

    Vector<ModelSubMesh> subMeshes;
    ModelSubMesh subMesh;
    subMesh.firstVertexIndex = 0;
    subMesh.vertexCount = static_cast<UInt32>(vertices.size());
    subMesh.firstIndexIndex = 0;
    subMesh.indexCount = static_cast<UInt32>(indices.size());
    subMesh.materialIndex = 0;
    subMeshes.push_back(subMesh);

    Vector<SharedPtr<AMaterialDescriptor>> materials;
    materials.push_back(m_materialDescriptorAssetManager.getDefault());

    return MakeShared<Model>(
      PrimitiveModelPathUtilities::GetPrimitiveModelPath(
        primitiveModelType::ConeNoBase
      ),
      vertices,
      indices,
      subMeshes,
      materials
    );
  }
}

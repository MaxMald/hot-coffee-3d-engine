#include "hc/editor/graphics/hcMeshShapeFactory.h"

namespace hc::editor
{
  SharedPtr<IMesh> MeshShapeFactory::CreateCircle(
    IMeshManager& meshManager,
    float radius,
    UInt32 segments,
    const Color& color
  )
  {
    segments = Math::Max((UInt32)3, segments);

    Buffer<Vertex> vertices((SizeT)segments);
    for (UInt32 i = 0; i < segments; ++i)
    {
      float angle = (static_cast<float>(i) / segments) * Math::TwoPi;
      float x = radius * Math::Cos(angle);
      float z = radius * Math::Sin(angle);
      vertices[i] = Vertex(
        Vector3f(x, 0.0f, z),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector2f(static_cast<float>(i) / segments, 1.0f)
      );
    }

    BufferUInt32 indices((SizeT)segments);
    for (UInt32 i = 0; i < segments; ++i)
      indices[i] = i;

    Vector<ModelSubMesh> subMeshes(1);
    subMeshes[0].firstVertexIndex = 0;
    subMeshes[0].vertexCount = segments;
    subMeshes[0].firstIndexIndex = 0;
    subMeshes[0].indexCount = segments;
    subMeshes[0].materialIndex = 0;

    SharedPtr<UnlitMaterialDescriptor> materialDescriptor = MakeShared<UnlitMaterialDescriptor>(
      Path(""),
      "UnlitMaterial",
      color,
      Path()
    );
    materialDescriptor->setDoubleSided(true);

    Vector<SharedPtr<AMaterialDescriptor>> materialDescriptors(1);
    materialDescriptors[0] = materialDescriptor;

    SharedPtr<Model> model = MakeShared<Model>(
      Path(""),
      vertices,
      indices,
      subMeshes,
      materialDescriptors
    );

    SharedPtr<IMesh> mesh = meshManager.createMeshFromModel(model);
    mesh->setDrawType(drawType::LineLoop);

    return mesh;
  }

  SharedPtr<IMesh> MeshShapeFactory::CreateLine(
    IMeshManager& meshManager,
    const Vector3f& start,
    const Vector3f& end,
    const Color& color
  )
  {
    Buffer<Vertex> vertices(2);
    vertices[0] = Vertex(
      start,
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 0.0f)
    );

    vertices[1] = Vertex(
      end,
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 1.0f)
    );

    BufferUInt32 indices(2);
    indices[0] = 0;
    indices[1] = 1;

    Vector<ModelSubMesh> subMeshes(1);
    subMeshes[0].firstVertexIndex = 0;
    subMeshes[0].vertexCount = 2;
    subMeshes[0].firstIndexIndex = 0;
    subMeshes[0].indexCount = 2;
    subMeshes[0].materialIndex = 0;

    SharedPtr<UnlitMaterialDescriptor> materialDescriptor = MakeShared<UnlitMaterialDescriptor>(
      Path(""),
      "UnlitMaterial",
      color,
      Path()
    );
    materialDescriptor->setDoubleSided(true);

    Vector<SharedPtr<AMaterialDescriptor>> materialDescriptors(1);
    materialDescriptors[0] = materialDescriptor;

    SharedPtr<Model> model = MakeShared<Model>(
      Path(""),
      vertices,
      indices,
      subMeshes,
      materialDescriptors
    );

    SharedPtr<IMesh> mesh = meshManager.createMeshFromModel(model);
    mesh->setDrawType(drawType::Lines);

    return mesh;
  }

  SharedPtr<IMesh> MeshShapeFactory::CreateRectangle(
    IMeshManager& meshManager,
    float width,
    float height,
    const Color& color
  )
  {
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    Buffer<Vertex> vertices(4);
    vertices[0] = Vertex(
      Vector3f(-halfWidth, 0.0f, -halfHeight),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 0.0f)
    );
    vertices[1] = Vertex(
      Vector3f(halfWidth, 0.0f, -halfHeight),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 0.0f)
    );
    vertices[2] = Vertex(
      Vector3f(halfWidth, 0.0f, halfHeight),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 1.0f)
    );
    vertices[3] = Vertex(
      Vector3f(-halfWidth, 0.0f, halfHeight),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 1.0f)
    );

    BufferUInt32 indices(4);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;

    Vector<ModelSubMesh> subMeshes(1);
    subMeshes[0].firstVertexIndex = 0;
    subMeshes[0].vertexCount = 4;
    subMeshes[0].firstIndexIndex = 0;
    subMeshes[0].indexCount = 4;
    subMeshes[0].materialIndex = 0;

    SharedPtr<UnlitMaterialDescriptor> materialDescriptor = MakeShared<UnlitMaterialDescriptor>(
      Path(""),
      "UnlitMaterial",
      color,
      Path()
    );

    Vector<SharedPtr<AMaterialDescriptor>> materialDescriptors(1);
    materialDescriptors[0] = materialDescriptor;

    SharedPtr<Model> model = MakeShared<Model>(
      Path(""),
      vertices,
      indices,
      subMeshes,
      materialDescriptors
    );

    SharedPtr<IMesh> mesh = meshManager.createMeshFromModel(model);
    mesh->setDrawType(drawType::LineLoop);

    return mesh;
  }

  SharedPtr<IMesh> MeshShapeFactory::CreateBox(
    IMeshManager& meshManager,
    float width,
    float height,
    float depth,
    const Color& color
  )
  {
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    float halfDepth = depth * 0.5f;

    Buffer<Vertex> vertices(8);
    vertices[0] = Vertex(
      Vector3f(-halfWidth, -halfHeight, -halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 0.0f)
    );
    vertices[1] = Vertex(
      Vector3f(halfWidth, -halfHeight, -halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 0.0f)
    );
    vertices[2] = Vertex(
      Vector3f(halfWidth, halfHeight, -halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 1.0f)
    );
    vertices[3] = Vertex(
      Vector3f(-halfWidth, halfHeight, -halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 1.0f)
    );
    vertices[4] = Vertex(
      Vector3f(-halfWidth, -halfHeight, halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 0.0f)
    );
    vertices[5] = Vertex(
      Vector3f(halfWidth, -halfHeight, halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 0.0f)
    );
    vertices[6] = Vertex(
      Vector3f(halfWidth, halfHeight, halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(1.0f, 1.0f)
    );
    vertices[7] = Vertex(
      Vector3f(-halfWidth, halfHeight, halfDepth),
      Vector3f(0.0f, 1.0f, 0.0f),
      Vector3f(1.0f, 0.0f, 0.0f),
      Vector2f(0.0f, 1.0f)
    );

    BufferUInt32 indices(24);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 1;
    indices[3] = 2;
    indices[4] = 2;
    indices[5] = 3;
    indices[6] = 3;
    indices[7] = 0;

    indices[8] = 4;
    indices[9] = 5;
    indices[10] = 5;
    indices[11] = 6;
    indices[12] = 6;
    indices[13] = 7;
    indices[14] = 7;
    indices[15] = 4;

    indices[16] = 0;
    indices[17] = 4;
    indices[18] = 1;
    indices[19] = 5;
    indices[20] = 2;
    indices[21] = 6;
    indices[22] = 3;
    indices[23] = 7;

    Vector<ModelSubMesh> subMeshes(1);
    subMeshes[0].firstVertexIndex = 0;
    subMeshes[0].vertexCount = 8;
    subMeshes[0].firstIndexIndex = 0;
    subMeshes[0].indexCount = 24;
    subMeshes[0].materialIndex = 0;

    SharedPtr<UnlitMaterialDescriptor> materialDescriptor = MakeShared<UnlitMaterialDescriptor>(
      Path(""),
      "UnlitMaterial",
      color,
      Path()
    );
    materialDescriptor->setDoubleSided(true);

    Vector<SharedPtr<AMaterialDescriptor>> materialDescriptors(1);
    materialDescriptors[0] = materialDescriptor;

    SharedPtr<Model> model = MakeShared<Model>(
      Path(""),
      vertices,
      indices,
      subMeshes,
      materialDescriptors
    );

    SharedPtr<IMesh> mesh = meshManager.createMeshFromModel(model);
    mesh->setDrawType(drawType::Lines);

    return mesh;
  }
}

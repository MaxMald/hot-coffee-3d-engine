#include "hc/graphics/hcDrawCommand.h"
#include "hc/assets/materialDescriptor/hcAMaterialDescriptor.h"

namespace hc
{
  void DrawCommand::initialize(
    const CameraMatrices& cameraMatrices,
    const Matrix4& modelMatrix,
    SharedPtr<IMaterial> material,
    float distanceToCamera,
    UInt32 firstIndex,
    UInt32 indexCount,
    const Variant<OpenGlDrawData>& apiDrawData
  )
  {
    this->cameraMatrices = cameraMatrices;
    this->modelMatrix = modelMatrix;
    this->material = material;
    this->firstIndex = firstIndex;
    this->indexCount = indexCount;
    this->apiDrawData = apiDrawData;

    if (!material)
      return;

    SharedPtr<AMaterialDescriptor> descriptor = material->getDescriptor();
    if (!descriptor)
      return;

    sortKey = GenerateSortKey(
      descriptor->getRenderMode(),
      material->getMaterialId(),
      distanceToCamera
    );
  }

  void DrawCommand::reset()
  {
    modelMatrix = Matrix4::Identity();
    material = nullptr;
    sortKey = 0;
    firstIndex = 0;
    indexCount = 0;
    apiDrawData = {};
  }

  UInt64 DrawCommand::GenerateSortKey(
    materialRenderMode::Type renderMode,
    UInt16 materialId,
    float distanceToCamera
  )
  {
    UInt64 key = 0;

    // Pack render mode into the first 8 bits (63-56)
    key |= static_cast<UInt64>(renderMode) << 56;

    // Pack material ID into the next 16 bits (55-40)
    key |= static_cast<UInt64>(materialId) << 40;

    // Convert float to sortable integer bits
    UInt32 distanceBits = bitUtilities::floatFlip(distanceToCamera);

    // For transparent materials, invert distance for back-to-front sorting
    if (renderMode == materialRenderMode::Type::Transparent)
      distanceBits = ~distanceBits;

    // Pack distance into the next 32 bits (39-8)
    key |= static_cast<UInt64>(distanceBits) << 8;

    // The last 8 bits (7-0) can be used for additional flags if needed
    return key;
  }
}

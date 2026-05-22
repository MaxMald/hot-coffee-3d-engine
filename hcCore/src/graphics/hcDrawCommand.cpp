#include "hc/graphics/hcDrawCommand.h"

namespace hc
{
  void DrawCommand::initialize(
    const CameraMatrices& _cameraMatrices,
    const Matrix4& _modelMatrix,
    SharedPtr<IMaterial> _material,
    float distanceToCamera,
    UInt32 _firstIndex,
    UInt32 _indexCount,
    polygonFillType::Type _polygonFillType,
    const Variant<OpenGlDrawData>& _apiDrawData
  )
  {
    this->cameraMatrices = _cameraMatrices;
    this->modelMatrix = _modelMatrix;
    this->material = _material;
    this->firstIndex = _firstIndex;
    this->indexCount = _indexCount;
    this->polygonFillType = _polygonFillType;
    this->apiDrawData = _apiDrawData;

    if (!_material)
      return;

    sortKey = GenerateSortKey(
      material->getRenderMode(),
      material->getMaterialId(),
      distanceToCamera
    );
  }

  void DrawCommand::reset()
  {
    modelMatrix = Matrix4::Identity();
    material = nullptr;
    sortKey = 0;
    polygonFillType = polygonFillType::Undefined;
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

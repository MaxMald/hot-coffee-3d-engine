#include "hc/graphics/resource/material/hcAMaterial.h"

namespace hc
{
  AMaterial::~AMaterial()
  {}

  const Id& AMaterial::getId() const
  {
    return m_id;
  }

  UInt16 AMaterial::getMaterialId() const
  {
    return m_materialId;
  }

  materialRenderMode::Type AMaterial::getRenderMode() const
  {
    return m_renderMode;
  }

  void AMaterial::setRenderMode(materialRenderMode::Type renderMode)
  {
    m_renderMode = renderMode;
  }

  float AMaterial::getAlphaCutoutThreshold() const
  {
    return m_alphaCutoutThreshold;
  }

  void AMaterial::setAlphaCutoutThreshold(float threshold)
  {
    m_alphaCutoutThreshold = Math::Min(Math::Max(threshold, 0.0f), 1.0f);
  }

  bool AMaterial::isDoubleSided() const
  {
    return m_doubleSided;
  }

  void AMaterial::setDoubleSided(bool doubleSided)
  {
    m_doubleSided = doubleSided;
  }

  AMaterial::AMaterial(
    UInt16 materialId,
    materialRenderMode::Type renderMode,
    float alphaCutoutThreshold,
    bool doubleSided
  ) :
    m_id(Id::Create()),
    m_materialId(materialId),
    m_renderMode(renderMode),
    m_alphaCutoutThreshold(alphaCutoutThreshold),
    m_doubleSided(doubleSided)
  {}
}

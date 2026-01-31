#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcAsset.h"
#include "hc/hcVertex.h"
#include "hc/hcBuffer.h"

namespace hc
{
  class HC_CORE_EXPORT Model : public Asset
  {
    Model();
    ~Model() override;

    const Buffer<Vertex>& getVertices() const;
    const BufferUInt32& getIndices() const;

  private:
    Buffer<Vertex> m_vertices;
    BufferUInt32 m_indices;
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  /**
   * @brief Represents a submesh within a Model.
   *
   * A ModelSubMesh defines a contiguous range of vertices and indices within the
   * parent Model's shared vertex and index buffers.
   */
  struct HC_CORE_EXPORT ModelSubMesh
  {
  public:
    /**
     * @brief Index of the first vertex of this submesh in the model's vertex
     * buffer.
     */
    UInt32 firstVertexIndex;

    /**
     * @brief Number of vertices in this submesh.
     */
    UInt32 vertexCount;

    /**
     * @brief Index of the first index of this submesh in the model's index
     * buffer.
     */
    UInt32 firstIndexIndex;

    /**
     * @brief Number of indices in this submesh.
     */
    UInt32 indexCount;

    /**
     * @brief Index of the material used by this submesh in the model's material
     * list.
     */
    UInt32 materialIndex;

    ModelSubMesh();
    ~ModelSubMesh();
  };
}

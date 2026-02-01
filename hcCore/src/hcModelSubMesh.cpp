#include "hc/hcModelSubMesh.h"

namespace hc
{
  ModelSubMesh::ModelSubMesh() :
    firstVertexIndex(0),
    vertexCount(0),
    firstIndexIndex(0),
    indexCount(0),
    materialIndex(0)
  {
  }

  ModelSubMesh::~ModelSubMesh()
  {
  }
}

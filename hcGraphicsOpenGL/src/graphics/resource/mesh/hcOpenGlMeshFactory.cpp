#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"

namespace hc
{
  SharedPtr<IMesh> OpenGlMeshFactory::createMesh(
    SharedPtr<Model> model,
    Vector<SharedPtr<IMaterial>>& materials
  )
  {
    return MakeShared<OpenGlMesh>(model, materials);
  }
}

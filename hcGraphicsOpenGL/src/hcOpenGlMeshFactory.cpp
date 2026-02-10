#include "hc/hcOpenGlMeshFactory.h"
#include "hc/hcOpenGlMesh.h"

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

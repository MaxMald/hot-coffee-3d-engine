#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"

namespace hc
{
  OpenGlMeshFactory::OpenGlMeshFactory(IGraphicsManager& graphicsManager) :
    m_graphicsManager(graphicsManager)
  {
  }

  SharedPtr<IMesh> OpenGlMeshFactory::createMesh(
    SharedPtr<Model> model,
    Vector<SharedPtr<IMaterial>>& materials
  )
  {
    return MakeShared<OpenGlMesh>(model, materials, m_graphicsManager);
  }
}

#include "hc/graphics/resource/mesh/hcOpenGlMeshFactory.h"
#include "hc/graphics/resource/mesh/hcOpenGlMesh.h"

namespace hc
{
  OpenGlMeshFactory::OpenGlMeshFactory(IGraphicsManager& graphicsManager) :
    m_graphicsManager(graphicsManager)
  {}

  SharedPtr<IMesh> OpenGlMeshFactory::createMesh()
  {
    return MakeShared<OpenGlMesh>(m_graphicsManager);
  }
}

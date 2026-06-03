#pragma once

#include <hc/graphics/resource/mesh/hcIMeshFactory.h>
#include "hc/hcGraphicsOpenGlPrerequisites.h"

namespace hc
{
  /**
   * @brief Factory for creating OpenGL mesh instances.
   */
  class OpenGlMeshFactory : public IMeshFactory
  {
  public:
    /**
     * @brief Constructs an OpenGlMeshFactory with a reference to the graphics manager.
     *
     * @param graphicsManager Reference to the IGraphicsManager used for resource
     * management.
     */
    OpenGlMeshFactory(IGraphicsManager& graphicsManager);
    ~OpenGlMeshFactory() override = default;

    /**
     * @brief Creates an OpenGL mesh instance.
     *
     * @return Shared pointer to the created IMesh instance. nullptr if creation fails.
     */
    SharedPtr<IMesh> createMesh() override;

  private:
    IGraphicsManager& m_graphicsManager;
  };
}

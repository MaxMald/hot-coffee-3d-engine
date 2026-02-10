#pragma once

#include <hc/hcIMeshFactory.h>

namespace hc
{
  /**
   * @brief Factory for creating OpenGL mesh instances.
   */
  class OpenGlMeshFactory : public IMeshFactory
  {
  public:
    OpenGlMeshFactory() = default;
    ~OpenGlMeshFactory() override = default;

    /**
     * @brief Creates an OpenGL mesh instance.
     * 
     * @param model Shared pointer to the model data.
     * @param materials Reference to a vector of shared pointers to materials.
     * 
     * @return Shared pointer to the created IMesh instance.
     */
    SharedPtr<IMesh> createMesh(
      SharedPtr<Model> model,
      Vector<SharedPtr<IMaterial>>& materials
    ) override;
  };
}

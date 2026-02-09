#pragma once

#include "hc/hcIGraphicResource.h"
#include "hc/hcIDrawable.h"

namespace hc
{
  class IMaterial;
  class Model;

  /**
   * @brief Interface for mesh objects in the engine.
   */
  class IMesh : public IGraphicResource, public IDrawable
  {
  public:
    virtual ~IMesh() = default;

    /**
     * @brief Returns the unique identifier of the mesh.
     * 
     * @return The unique Id of the mesh.
     */
    virtual const Id& getId() const = 0;

    /**
     * @brief Returns the model associated with this mesh.
     *
     * @return Shared pointer to the associated Model.
     */
    virtual SharedPtr<Model> getModel() const = 0;

    /**
     * @brief Updates the mesh state.
     */
    virtual void update() = 0;

    /**
     * @brief Destroys the mesh and releases its resources.
     */
    virtual void destroy() = 0;

    /**
     * @brief Returns the materials associated with this mesh.
     * 
     * @return A vector of shared pointers to the materials.
     */
    virtual const Vector<SharedPtr<IMaterial>> getMaterials() = 0;

  protected:
    /**
     * @brief Protected default constructor to prevent direct instantiation.
     */
    IMesh() = default;
  };
}

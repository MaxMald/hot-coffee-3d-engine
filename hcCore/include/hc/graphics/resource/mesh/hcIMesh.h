#pragma once

#include "hc/graphics/resource/hcIGraphicResource.h"
#include "hc/graphics/hcIDrawable.h"

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
     * @brief Sets the material at the specified index for this mesh.
     *
     * @param index The index of the material to set.
     * @param material Shared pointer to the material to associate with this mesh.
     */
    virtual void setMaterial(UInt32 index, SharedPtr<IMaterial> material) = 0;

    /**
    * @brief Returns the number of materials associated with this mesh.
    */
    virtual SizeT getMaterialsSize() const = 0;

    /**
     * @brief Returns the materials associated with this mesh.
     * 
     * @return A vector of shared pointers to the materials.
     */
    virtual const Vector<SharedPtr<IMaterial>> getMaterials() = 0;

  protected:
    IMesh() = default;
  };
}

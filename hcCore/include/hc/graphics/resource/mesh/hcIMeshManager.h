#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IMesh;
  class Model;

  /**
   * @brief Interface for mesh manager classes.
   *
   * Provides an abstract interface for managing mesh resources, including
   * creation, retrieval, and cleanup of mesh objects. Mesh managers are
   * non-copyable.
   */
  class HC_CORE_EXPORT IMeshManager : public NonCopyable
  {
  public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IMeshManager() = default;

    /**
     * @brief Creates a mesh from a file path.
     * 
     * @param path The file path to load the mesh from.
     * 
     * @return Shared pointer to the created mesh.
     */
    virtual SharedPtr<IMesh> createMeshFromPath(const Path& path) = 0;

    /**
     * @brief Creates a mesh from a model instance.
     * 
     * @param model Shared pointer to the model to create the mesh from.
     * 
     * @return Shared pointer to the created mesh.
     */
    virtual SharedPtr<IMesh> createMeshFromModel(const SharedPtr<Model>& model) = 0;

    /**
     * @brief Returns a constant reference to the list of all managed meshes.
     * 
     * @return Vector of shared pointers to all meshes.
     */
    virtual const Vector<SharedPtr<IMesh>>& getMeshes() const = 0;

    /**
     * @brief Clears all managed meshes and releases their resources.
     */
    virtual void clear() = 0;

  protected:
    /**
     * @brief Protected default constructor to prevent direct instantiation.
     */
    IMeshManager() = default;
  };
}

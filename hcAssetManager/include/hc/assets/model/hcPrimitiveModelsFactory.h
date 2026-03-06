#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Model;
  class IMaterialDescriptorAssetManager;

  /**
   * Factory for creating primitive 3D model shapes.
   * Provides static methods to generate common geometric primitives.
   */
  class PrimitiveModelsFactory
  {
  public:
    PrimitiveModelsFactory(
      IMaterialDescriptorAssetManager& materialDescriptorAssetManager
    );
    ~PrimitiveModelsFactory() = default;

    /**
    * Creates a model based on the specified primitive type.
    *
    * @param primitiveType The type of primitive model to create.
    * 
    * @return A shared pointer to the created model.
    */
    SharedPtr<Model> createFromPrimitiveType(
      primitiveModelType::Type primitiveType
    );

    /**
     * Creates a cube model with unit dimensions.
     *
     * @return A shared pointer to the created cube model.
     */
    SharedPtr<Model> createCube();

    /**
     * Creates a cone model with default dimensions.
     *
     * @return A shared pointer to the created cone model.
     */
    SharedPtr<Model> createCone();

    /**
     * Creates a sphere model with default dimensions.
     *
     * @return A shared pointer to the created sphere model.
     */
    SharedPtr<Model> createSphere();

    /**
     * Creates a cylinder model with default dimensions.
     *
     * @return A shared pointer to the created cylinder model.
     */
    SharedPtr<Model> createCylinder();

    /**
     * Creates a plane model with default dimensions.
     *
     * @return A shared pointer to the created plane model.
     */
    SharedPtr<Model> createPlane();

    /**
     * Creates a pyramid model with default dimensions.
     *
     * @return A shared pointer to the created pyramid model.
     */
    SharedPtr<Model> createPyramid();

  private:
    IMaterialDescriptorAssetManager& m_materialDescriptorAssetManager;
  };
}

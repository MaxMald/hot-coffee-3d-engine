#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Model;

  /**
   * Factory for creating primitive 3D model shapes.
   * Provides static methods to generate common geometric primitives.
   */
  struct PrimitiveModelsFactory
  {
    /**
     * Creates a cube model with unit dimensions.
     *
     * @return A shared pointer to the created cube model.
     */
    static SharedPtr<Model> createCube();

    /**
     * Creates a cone model with default dimensions.
     *
     * @return A shared pointer to the created cone model.
     */
    static SharedPtr<Model> createCone();

    /**
     * Creates a sphere model with default dimensions.
     *
     * @return A shared pointer to the created sphere model.
     */
    static SharedPtr<Model> createSphere();

    /**
     * Creates a cylinder model with default dimensions.
     *
     * @return A shared pointer to the created cylinder model.
     */
    static SharedPtr<Model> createCylinder();

    /**
     * Creates a plane model with default dimensions.
     *
     * @return A shared pointer to the created plane model.
     */
    static SharedPtr<Model> createPlane();

    /**
     * Creates a pyramid model with default dimensions.
     *
     * @return A shared pointer to the created pyramid model.
     */
    static SharedPtr<Model> createPyramid();
  };
}

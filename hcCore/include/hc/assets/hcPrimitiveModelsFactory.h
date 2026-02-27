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

    static SharedPtr<Model> createSphere();

    static SharedPtr<Model> createCylinder();

    static SharedPtr<Model> createPlane();

    static SharedPtr<Model> createPiramid();
  };
}

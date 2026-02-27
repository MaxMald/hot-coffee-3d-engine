#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IMaterial;
  class ITextureManager;
  class IShaderProgramManager;
  class AMaterialDescriptor;

  /**
   * @brief Interface for material factory classes.
   *
   * Provides an interface for creating IMaterial instances from a
   * MaterialDescriptor. Implementations of this interface are responsible for
   * constructing materials according to the provided descriptor.
   */
  class IMaterialFactory
  {
  public:
    virtual ~IMaterialFactory() = default;

    /**
     * @brief Creates a new IMaterial instance based on the given descriptor.
     *
     * @param descriptor Shared pointer to a MaterialDescriptor describing the
     * material to create.
     * @param textureManager Reference to the ITextureManager for loading any
     * required textures.
     * @param shaderProgramManager Reference to the IShaderProgramManager for
     * managing shader programs.
     *
     * @return Shared pointer to the created IMaterial instance.
     */
    virtual SharedPtr<IMaterial> create(
      SharedPtr<AMaterialDescriptor> descriptor,
      ITextureManager& textureManager,
      IShaderProgramManager& shaderProgramManager
    ) = 0;

  protected:
    IMaterialFactory() = default;
  };
}

#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IShaderFactory;
  class IShaderProgramFactory;
  class IMeshFactory;
  class ITextureFactory;

  /**
   * @brief Interface for accessing graphic resource factories.
   *
   * Provides access to factories responsible for creating shaders, shader
   * programs, meshes, and textures. Implementations should supply concrete
   * factory instances for each resource type.
   */
  class IGraphicResourcesFactories
  {
  public:
    virtual ~IGraphicResourcesFactories() = default;

    /**
     * @brief Returns the shader factory instance.
     * 
     * @return Reference to the shader factory.
     */
    virtual IShaderFactory& getShaderFactory() const = 0;

    /**
     * @brief Returns the shader program factory instance.
     * 
     * @return Reference to the shader program factory.
     */
    virtual IShaderProgramFactory& getShaderProgramFactory() const = 0;

    /**
     * @brief Returns the mesh factory instance.
     * 
     * @return Reference to the mesh factory.
     */
    virtual IMeshFactory& getMeshFactory() const = 0;

    /**
     * @brief Returns the texture factory instance.
     * 
     * @return Reference to the texture factory.
     */
    virtual ITextureFactory& getTextureFactory() const = 0;

  private:
    IGraphicResourcesFactories() = default;
  };
}

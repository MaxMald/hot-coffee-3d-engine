#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class ICubeMap;
  class Image;

  /**
   * @brief Represents a skybox in the scene.
   *
   * The Skybox class encapsulates a cube map texture that can be used to render a skybox
   * in a 3D scene.
   */
  class HC_CORE_EXPORT Skybox : public NonCopyable
  {
  public:
    Path sourcePath; ///< The source path of the skybox texture. Could be from an equirectangular image or a cube map descriptor.

    Skybox();
    ~Skybox();

    /**
     * @brief Initializes the skybox with the specified cube map.
     * @param cubeMap The cube map to be used for the skybox.
     * @param sourcePath The source path of the skybox texture.
     */
    void initialize(SharedPtr<ICubeMap> cubeMap, const Path& sourcePath = Path());

    /**
     * @brief Checks if the skybox is valid.
     * @return True if the skybox is valid, false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Checks if the skybox has a valid cube map.
     * @return True if the skybox has a valid cube map, false otherwise.
     */
    bool hasCubeMap() const;

    /**
     * @brief Gets the cube map associated with the skybox.
     * @return Reference to the cube map.
     */
    const ICubeMap& getCubeMap() const;

    /**
     * @brief Gets the cube map associated with the skybox.
     * @return Reference to the cube map.
     */
    ICubeMap& getCubeMap();

    /**
     * @brief Clears the skybox, releasing any associated resources.
     */
    void clear();

    /**
     * @brief Destroys the skybox, releasing any associated resources.
     */
    void destroy();

  protected:
    SharedPtr<ICubeMap> m_cubeMap;
  };
}

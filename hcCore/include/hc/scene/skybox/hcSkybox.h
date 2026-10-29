#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/generators/hcIEquirectangularToCubeMapGenerator.h"
#include "hc/graphics/generators/hcIDiffuseIrradianceCubeMapGenerator.h"
#include "hc/graphics/generators/hcIPrefilteredEnvironmentCubeMapGenerator.h"

namespace hc
{
  class IAssetManager;
  class IGraphicsManager;
  class ICubeMap;
  class Image;

  /**
   * @brief Represents the settings for a skybox.
   *
   * The SkyboxSettings struct encapsulates the configuration required to create a skybox
   * in a 3D scene. It includes the source path of the skybox texture, which can be an
   * equirectangular image or a cube map descriptor, as well as settings for generating
   * the skybox's cube map and diffuse irradiance cube map.
   */
  struct HC_CORE_EXPORT SkyboxSettings : public io::ISerializable
  {
    Path sourcePath; ///< The source path of the skybox texture, can be an equirectangular image or cube map descriptor.
    graphics::generators::EquirectangularToCubeMapGeneratorSettings cubeMapGeneratorSettings;                             ///< Settings for generating the skybox's cube map from an equirectangular image.
    graphics::generators::DiffuseIrradianceCubeMapGeneratorSettings diffuseIrradianceCubeMapGeneratorSettings;            ///< Settings for generating the skybox's diffuse irradiance cube map.
    graphics::generators::PrefilteredEnvironmentCubeMapGeneratorSettings prefilteredEnvironmentCubeMapGeneratorSettings;  ///< Settings for generating the skybox's prefiltered environment cube map.

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;
    void clear();
  };

  /**
   * @brief Represents a skybox in the scene.
   *
   * The Skybox class encapsulates a cube map texture that can be used to render a skybox
   * in a 3D scene.
   */
  class HC_CORE_EXPORT Skybox : public NonCopyable
  {
  public:
    Skybox();
    ~Skybox();

    /**
     * @brief Initializes the skybox with the specified cube maps.
     * @param cubeMap The cube map to be used for the skybox.
     * @param diffuseIrradianceCubeMap The diffuse irradiance cube map to be used for the
     * skybox.
     * @param prefilteredEnvironmentCubeMap The prefiltered environment cube map to be
     * used for the skybox.
     * @throw RuntimeErrorException
     */
    void initialize(
      SharedPtr<ICubeMap> cubeMap,
      SharedPtr<ICubeMap> diffuseIrradianceCubeMap,
      SharedPtr<ICubeMap> prefilteredEnvironmentCubeMap
    );

    /**
     * @brief Initializes the skybox using the provided settings and managers.
     * @param assetManager Reference to the asset manager for loading assets.
     * @param graphicsManager Reference to the graphics manager for creating graphics resources.
     * @param settings The settings to configure the skybox.
     * @throw RuntimeErrorException
     */
    void initialize(
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager,
      SkyboxSettings& settings
    );

    /**
     * @brief Checks if the skybox is valid.
     * @return True if the skybox is valid, false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Checks if the skybox has a valid cube map.
     * @return True if the skybox has a valid cube map, false otherwise.
     */
    inline bool hasCubeMap() const
    {
      return m_cubeMap != nullptr;
    }

    /**
     * @brief Gets the cube map associated with the skybox.
     * @return Reference to the cube map.
     */
    inline const ICubeMap& getCubeMap() const
    {
      if (m_cubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getCubeMap: CubeMap is undefined.");
      return *m_cubeMap;
    }

    /**
     * @brief Gets the cube map associated with the skybox.
     * @return Reference to the cube map.
     */
    inline ICubeMap& getCubeMap()
    {
      if (m_cubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getCubeMap: CubeMap is undefined.");
      return *m_cubeMap;
    }

    /**
     * @brief Gets the shared pointer to the cube map associated with the skybox.
     * @return Shared pointer to the cube map.
     */
    inline SharedPtr<ICubeMap> getCubeMapPtr() const
    {
      return m_cubeMap;
    }

    /**
     * @brief Gets the diffuse irradiance cube map associated with the skybox.
     * @return Reference to the diffuse irradiance cube map.
     */
    inline const ICubeMap& getDiffuseIrradianceCubeMap() const
    {
      if (m_diffuseIrradianceCubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getDiffuseIrradianceCubeMap: DiffuseIrradianceCubeMap is undefined.");
      return *m_diffuseIrradianceCubeMap;
    }

    /**
     * @brief Gets the diffuse irradiance cube map associated with the skybox.
     * @return Reference to the diffuse irradiance cube map.
     */
    inline ICubeMap& getDiffuseIrradianceCubeMap()
    {
      if (m_diffuseIrradianceCubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getDiffuseIrradianceCubeMap: DiffuseIrradianceCubeMap is undefined.");
      return *m_diffuseIrradianceCubeMap;
    }

    /**
     * @brief Gets the shared pointer to the diffuse irradiance cube map associated with
     * the skybox.
     * @return Shared pointer to the diffuse irradiance cube map.
     */
    inline SharedPtr<ICubeMap> getDiffuseIrradianceCubeMapPtr() const
    {
      return m_diffuseIrradianceCubeMap;
    }

    /**
     * @brief Gets the prefiltered environment cube map associated with the skybox.
     * @return Reference to the prefiltered environment cube map.
     */
    inline const ICubeMap& getPrefilteredEnvironmentCubeMap() const
    {
      if (m_prefilteredEnvironmentCubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getPrefilteredEnvironmentCubeMap: PrefilteredEnvironmentCubeMap is undefined.");
      return *m_prefilteredEnvironmentCubeMap;
    }

    /**
     * @brief Gets the prefiltered environment cube map associated with the skybox.
     * @return Reference to the prefiltered environment cube map.
     */
    inline ICubeMap& getPrefilteredEnvironmentCubeMap()
    {
      if (m_prefilteredEnvironmentCubeMap == nullptr)
        throw RuntimeErrorException("Skybox::getPrefilteredEnvironmentCubeMap: PrefilteredEnvironmentCubeMap is undefined.");
      return *m_prefilteredEnvironmentCubeMap;
    }

    /**
     * @brief Gets the shared pointer to the prefiltered environment cube map associated
     * with the skybox.
     * @return Shared pointer to the prefiltered environment cube map.
     */
    inline SharedPtr<ICubeMap> getPrefilteredEnvironmentCubeMapPtr() const
    {
      return m_prefilteredEnvironmentCubeMap;
    }

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
    SharedPtr<ICubeMap> m_diffuseIrradianceCubeMap;
    SharedPtr<ICubeMap> m_prefilteredEnvironmentCubeMap;
  };
}

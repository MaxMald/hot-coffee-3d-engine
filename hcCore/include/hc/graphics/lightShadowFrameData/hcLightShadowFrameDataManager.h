#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/graphics/lightShadowFrameData/hcLightShadowFrameData.h"

namespace hc
{
  class DirectionalLight;

  class HC_CORE_EXPORT LightShadowFrameDataManager
  {
  public:
    static constexpr SizeT DIRECTIONAL_SHADOW_MAP_TEXTURE_SIZE = 2048;  ///< Size of the shadow map texture (width and height).

    LightShadowFrameDataManager();
    ~LightShadowFrameDataManager();

    /**
     * @brief Initializes the light shadow frame data manager. This method sets up the necessary
     * resources and prepares the manager for use.
     */
    void initialize();

    /**
     * @brief Clears the light shadow frame data. This method resets the light shadow
     * frame data to its default state, effectively removing any previously stored shadow
     * data.
     */
    void clearLightShadowFrameData();

    /**
     * @brief Retrieves the light shadow frame data.
     *
     * @return A constant reference to the light shadow frame data.
     */
    inline const LightShadowFrameData& getLightShadowFrameData() const
    {
      return m_lightShadowFrameData;
    }

    /**
     * @brief Checks if the maximum number of directional light shadows has been reached.
     *
     * @return True if the maximum number of directional light shadows has been reached,
     * false otherwise.
     */
    inline bool hasReachedMaxDirectionalLightShadows() const
    {
      return m_countDirectionalLightShadows >= LightShadowFrameData::MAX_DIRECTIONAL_LIGHTS_SHADOW_DATA;
    }

    /**
     * @brief Generates data for a new directional light shadow and saves it in the light
     * shadow frame data. Returns the index of the newly generated shadow data, or -1 if
     * the maximum number of shadows has been reached.
     *
     * @param directionalLight The directional light for which to generate shadow data.
     *
     * @return The index of the newly generated directional light shadow data, or -1 if
     * the maximum number of shadows has been reached.
     */
    Int32 generateDirectionalLightShadowData(const DirectionalLight& directionalLight);

    /**
     * @brief Destroys the light shadow manager and releases any allocated resources.
     */
    void destroy();

  private:
    LightShadowFrameData m_lightShadowFrameData;
    SizeT m_countDirectionalLightShadows;
  };
}

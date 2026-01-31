#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcLightType.h"

namespace hc
{
  class Light;

  /**
   * @brief Manages the lifecycle and storage of Light objects in the engine.
   */
  class HC_CORE_EXPORT LightManager :
    private NonCopyable
  {
  public:
    /**
     * @brief Constructs a LightManager.
     */
    LightManager();

    /**
     * @brief Destroys the LightManager and all managed lights.
     */
    ~LightManager();

    /**
     * @brief Creates a new Light of the specified type.
     * 
     * @param type The type of light to create (default is Point).
     * 
     * @return Pointer to the created Light.
     */
    Light* createLight(lightType::Type type = lightType::Type::Point);

    /**
     * @brief Destroys a managed Light.
     * 
     * @param light Pointer to the Light to destroy.
     */
    void destroyLight(Light* light);

    /**
     * @brief Removes and destroys all managed lights.
     */
    void clear();

    /**
     * @brief Gets the list of all managed lights.
     * 
     * @return A const reference to the vector of unique pointers to Light
     * objects.
     */
    const Vector<UniquePtr<Light>>& getLights() const;

  private:
    Vector<UniquePtr<Light>> m_lights;
  };
}

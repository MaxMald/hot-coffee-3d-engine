#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/hcShadingType.h"

namespace hc
{
  class IMaterialFactory;

  /**
   * @brief Manages registration and retrieval of material factories by shading
   * type.
   *
   * This class allows registering, querying, and accessing IMaterialFactory
   * instances associated with specific shading types.
   */
  class MaterialFactoriesManager
  {
  public:
    /**
     * @brief Constructs a new MaterialFactoriesManager.
     */
    MaterialFactoriesManager();

    /**
     * @brief Destroys the MaterialFactoriesManager and all registered factories.
     */
    ~MaterialFactoriesManager();

    /**
     * @brief Registers a material factory for a given shading type.
     * 
     * @param shaderType The shading type to associate with the factory.
     * @param factory Unique pointer to the IMaterialFactory to register.
     */
    void registerFactory(
      shadingType::Type shaderType,
      UniquePtr<IMaterialFactory> factory
    );

    /**
     * @brief Checks if a factory is registered for the given shading type.
     * 
     * @param shaderType The shading type to check.
     * 
     * @return True if a factory is registered, false otherwise.
     */
    bool hasFactory(shadingType::Type shaderType) const;

    /**
     * @brief Retrieves the factory associated with the given shading type.
     *
     * @param shaderType The shading type whose factory is requested.
     *
     * @return Reference to the registered IMaterialFactory.
     *
     * @throws std::out_of_range if no factory is registered for the given type.
     */
    IMaterialFactory& getFactory(shadingType::Type shaderType) const;

  private:
    /**
     * @brief Maps shading types to their corresponding material factories.
     */
    UnorderedMap<shadingType::Type, UniquePtr<IMaterialFactory>> m_factoriesByShaderType;
  };
}

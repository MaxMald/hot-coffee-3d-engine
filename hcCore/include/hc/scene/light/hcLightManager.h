#pragma once

#include "hc/hcCorePrerequisites.h"
#include "hc/scene/light/hcLightType.h"

namespace hc
{
  class Light;

  /**
   * @brief Manages the lifecycle and storage of Light objects in the scene.
   */
  class HC_CORE_EXPORT LightManager :
    public NonCopyable,
    public ISerializable
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
     * @brief Serializes the LightManager and all managed lights to binary
     * format.
     *
     * Writes the number of lights, followed by the serialized data of each
     * light.
     * 
     * @param writer The BinaryWriter to use for serialization.
     */
    void serialize(BinaryWriter& writer) const override;

    /**
     * @brief Deserializes the LightManager and all managed lights from
     * binary format.
     *
     * Clears all existing lights and reads the number of lights, followed
     * by the serialized data of each light.
     * 
     * @param reader The BinaryReader to use for deserialization.
     */
    void deserialize(BinaryReader& reader) override;

    /**
     * @brief Creates a new Light of the specified type.
     * 
     * @param type The type of light to create (default is Point).
     * 
     * @return Pointer to the created Light.
     */
    Light* createLight(lightType::Type type = lightType::Type::Point);

    /**
     * @brief Deserializes a Light from binary format.
     *
     * Reads the light data from the provided BinaryReader and creates a new
     * Light object with the deserialized state. The caller takes ownership of
     * the returned Light pointer and is responsible for its destruction.
     * 
     * @param reader The BinaryReader to use for deserialization.
     * 
     * @return Pointer to the deserialized Light, or nullptr if deserialization
     * fails.
     */
    Light* deserializeLight(BinaryReader& reader);

    /**
     * @brief Adds an existing Light to the manager.
     * 
     * @param light The UniquePtr to the Light to add.
     */
    void addLight(UniquePtr<Light> light);

    /**
     * @brief Destroys a light with the specified ID.
     *
     * Removes and destroys the light from the managed lights. Logs an
     * error if the light is not found.
     * 
     * @param lightId The UUID of the light to destroy.
     */
    void destroyLight(const UUID& lightId);

    /**
     * @brief Gets a light by its ID.
     * 
     * @param lightId The UUID of the light to retrieve.
     * 
     * @return Pointer to the light, or nullptr if not found.
     */
    Light* getLight(const UUID& lightId) const;

    /**
     * @brief Removes and destroys all managed lights.
     */
    void clear();

    /**
     * @brief Gets a list of all managed lights.
     *
     * The list will be filled with pointers to the lights, but the caller
     * @b does @b not take ownership of the lights. The list will be cleared
     * before filling.
     * 
     * @param outLights Vector to be filled with pointers to all managed
     * lights.
     */
    void getLights(Vector<Light*>& outLights) const;

  private:
    UnorderedMap<UUID, UniquePtr<Light>> m_lights;
  };
}

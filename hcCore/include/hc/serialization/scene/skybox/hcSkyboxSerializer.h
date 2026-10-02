#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Skybox;
  class IAssetManager;
  class IGraphicsManager;

  namespace serialization
  {
    /**
     * @brief Specialized serializer for the Skybox class.
     */
    struct HC_CORE_EXPORT SkyboxSerializer
    {
    public:

      /**
       * @brief Serializes a Skybox object to a binary format.
       * 
       * @param skybox The Skybox object to serialize.
       * @param writer The BinaryWriter used for writing the serialized data.
       * @param assetManager The asset manager used for managing assets during serialization.
       */
      static void Serialize(
        const Skybox& skybox,
        io::BinaryWriter& writer,
        const IAssetManager& assetManager
      );

      /**
       * @brief Deserializes a Skybox object from a binary format.
       * 
       * @param skybox The Skybox object to populate with deserialized data.
       * @param reader The BinaryReader used for reading the serialized data.
       * @param assetManager The asset manager used for managing assets during
       * deserialization.
       * @param graphicsManager The graphics manager used for managing graphics resources
       * during deserialization.
       */
      static void Deserialize(
        Skybox& skybox,
        io::BinaryReader& reader,
        IAssetManager& assetManager,
        IGraphicsManager& graphicsManager
      );
    };
  }
}

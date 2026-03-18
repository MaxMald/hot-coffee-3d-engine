#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class Scene;

  namespace serialization
  {
    /**
     * @brief Serializer for .hcscene files.
     *
     * This class handles the serialization and deserialization of scene data
     * to and from the .hcscene file format. It ensures that the file format
     * version is validated and that the scene data is correctly read and
     * written according to the defined structure.
     */
    struct HC_CORE_EXPORT SceneSerializer
    {
    public:
      /**
       * @brief Serializes a scene to a .hcscene file.
       *
       * Writes the file header (magic number, format version, engine version)
       * followed by the complete scene data including lights, cameras, and the
       * scene graph hierarchy. Returns false if the file cannot be opened or
       * if an exception occurs during serialization.
       *
       * @param scene The scene to serialize.
       * @param filePath The path to the output .hcscene file.
       *
       * @return True if serialization was successful, false otherwise.
       */
      static bool Serialize(const Scene& scene, const Path& filePath);

      /**
       * @brief Deserializes a scene from a .hcscene file.
       *
       * Reads and validates the file header, then populates the provided
       * scene object with the deserialized data. The existing scene content
       * is cleared before deserialization. Returns false if the file cannot
       * be opened, has an invalid format, or if an exception occurs.
       *
       * @param filePath The path to the input .hcscene file.
       * @param scene Reference to the Scene object to populate with
       * deserialized data.
       *
       * @return True if deserialization was successful, false otherwise.
       */
      static bool Deserialize(const Path& filePath, Scene& scene);

      /**
       * @brief Deserializes a scene from a .hcscene file and returns a new
       * Scene instance.
       *
       * Creates a new Scene object and deserializes the file content into it.
       * Returns nullptr if the file cannot be opened, has an invalid format,
       * or if an exception occurs during deserialization.
       *
       * @param filePath The path to the input .hcscene file.
       *
       * @return A unique pointer to the deserialized Scene object, or nullptr
       * on failure.
       */
      static UniquePtr<Scene> Deserialize(const Path& filePath);

    private:
      /**
       * @brief Serializes the .hcscene file header.
       *
       * Writes the magic number, format version, and engine version to the
       * binary writer for file format identification and validation.
       *
       * @param writer The BinaryWriter to write the header to.
       */
      static void SerializeHeader(BinaryWriter& writer);

      /**
       * @brief Verifies the .hcscene file header.
       *
       * Reads and validates the magic number and format version from the
       * binary reader. Logs a warning if the file was created with a newer
       * engine version.
       *
       * @param reader The BinaryReader to read the header from.
       *
       * @throws RuntimeErrorException if the magic number is invalid or if
       * the file format version is unsupported.
       */
      static void VerifyHeader(BinaryReader& reader);
    };
  }
}

#pragma once

#include "hc/hcIAssetLoader.h"
#include "hc/hcJsonSerializer.h"

namespace hc
{
  /**
   * @brief Generic asset loader that deserializes objects from JSON files.
   * 
   * This template class implements the IAssetLoader interface for any type T
   * that can be deserialized using JsonSerializer. It loads the file content
   * from disk and attempts to deserialize it into an object of type T.
   * 
   * @tparam T The asset type to load and deserialize from JSON.
   */
  template <typename T>
  class JsonDeserializerAssetLoader :
    public IAssetLoader<T>
  {
  public:
    /**
     * @brief Loads and deserializes an asset from a JSON file.
     * 
     * Reads the file at the given path, deserializes its JSON content into an
     * object of type T, and returns a SharedPtr to the resulting object.
     * Returns nullptr if the file cannot be read or deserialization fails.
     * 
     * @param path The file path to load the asset from.
     * 
     * @return SharedPtr<T> The loaded asset, or nullptr on failure.
     */
    SharedPtr<T> load(const Path& path) override;
  };

  template <typename T>
  SharedPtr<T> JsonDeserializerAssetLoader<T>::load(const Path& path)
  {
    Optional<String> fileContent = fileUtilities::LoadStringFromFile(path);
    if (!fileContent.has_value())
      return nullptr;

    T* asset = JsonSerializer::Deserialize<T>(fileContent.value());
    return SharedPtr<T>(asset);
  }
}

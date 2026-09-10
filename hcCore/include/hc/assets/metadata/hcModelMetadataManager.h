#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class MaterialDescriptor;
  class Model;
}

namespace hc::assets::metadata
{
  struct HC_CORE_EXPORT ModelMetadata : public io::ISerializable
  { 
    Vector<SharedPtr<MaterialDescriptor>> materials;

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;
    void clear();
  };

  /**
   * @brief Manages the loading and saving of model metadata.
   */
  class HC_CORE_EXPORT ModelMetadataManager
  {
  public:
    static bool HasMetadata(const Path& modelPath);
    static void LoadMetadata(const Path& modelPath, Model& model);
    static void SaveMetadata(const Path& modelPath, const Model& model);

  private:
    static Path GetMetadataFilePath(const Path& modelPath);
    static void LoadMetadata(const ModelMetadata& metadata, Model& model);
  };
}

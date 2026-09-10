#include "hc/assets/metadata/hcModelMetadataManager.h"

#include "hc/assets/metadata/hcMetadataFileFormat.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"
#include "hc/assets/model/hcModel.h"

namespace hc::assets::metadata
{
  static constexpr UInt32 MODEL_METADATA_VERSION = 1;

  void ModelMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, MODEL_METADATA_VERSION);

    Vector<SharedPtr<MaterialDescriptor>> validMaterials;
    for (const auto& material : materials)
    {
      if (material)
        validMaterials.push_back(material);
    }

    writer.writeSizeT(validMaterials.size());
    for (const auto& material : validMaterials)
      material->serialize(writer);
    writer.finishWritingObject();
  }

  void ModelMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(0, MODEL_METADATA_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    // Material descriptors deserialization
    SizeT materialCount = reader.readSizeT();
    materials.resize(materialCount);
    for (SizeT i = 0; i < materialCount; ++i)
    {
      SharedPtr<MaterialDescriptor> mat = MakeShared<MaterialDescriptor>();
      mat->deserialize(reader);
      materials[i] = mat;
    }

    reader.finishReadingObject();
  }

  void ModelMetadata::clear()
  {
    materials.clear();
  }

  bool ModelMetadataManager::HasMetadata(const Path& modelPath)
  {
    String stringPath = modelPath.toGenericString();
    stringPath += hc::assets::metadata::fileFormat::Model::FILE_EXTENSION;
    Path metadataPath(stringPath);
    return metadataPath.exists();
  }

  void ModelMetadataManager::LoadMetadata(const Path& modelPath, Model& model)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.exists())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::loadMetadata: Metadata file does not exist for model: %s",
          modelPath.toGenericString().c_str()
        )
      );
    }

    try
    {
      String error;
      io::BinaryReader reader;
      if (!reader.prepare(metadataPath, error))
        throw IOException("Failed to open file. Error: " + error);

      ModelMetadata metadata;
      metadata.deserialize(reader);
      reader.shutdown();

      LoadMetadata(metadata, model);
    }
    catch (const Exception& ex)
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::loadMetadata: Failed to load metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  void ModelMetadataManager::SaveMetadata(const Path& modelPath, const Model& model)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.isCreatable())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::saveMetadata: Metadata file is not creatable for model: %s",
          modelPath.toGenericString().c_str()
        )
      );
    }

    try
    {
      ModelMetadata metadata;
      metadata.materials = model.getMaterials();

      String error;
      io::BinaryWriter writer;

      if (!writer.prepare(metadataPath, error))
        throw IOException("Failed to open file. Error: " + error);

      metadata.serialize(writer);
      writer.shutdown();
    }
    catch (const Exception& ex)
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::saveMetadata: Failed to save metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  Path ModelMetadataManager::GetMetadataFilePath(const Path& modelPath)
  {
    String stringPath = modelPath.toGenericString();
    stringPath += hc::assets::metadata::fileFormat::Model::FILE_EXTENSION;
    return Path(stringPath);
  }

  void ModelMetadataManager::LoadMetadata(const ModelMetadata& metadata, Model& model)
  {
    for (const SharedPtr<MaterialDescriptor>& metaMat : metadata.materials)
    {
      if (!metaMat)
        continue;

      SharedPtr<MaterialDescriptor> modelMat = model.getMaterial(metaMat->name);
      if (!modelMat)
      {
        LogService::Warning(
          String::Format(
            "ModelMetadataManager::loadMetadata: Material '%s' not found in model '%s'. Skipping.",
            metaMat->name.c_str(),
            model.path.toGenericString().c_str()
          )
        );
        continue;
      }

      if (modelMat->getType() != metaMat->getType())
      {
        LogService::Warning(
          String::Format(
            "ModelMetadataManager::loadMetadata: Material type mismatch for '%s' in model '%s'. Skipping.",
            metaMat->name.c_str(),
            model.path.toGenericString().c_str()
          )
        );
        continue;
      }

      *modelMat = *metaMat; // Override material properties based on type
    }
  }
}

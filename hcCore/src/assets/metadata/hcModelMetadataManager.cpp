#include "hc/assets/metadata/hcModelMetadataManager.h"

#include "hc/assets/hcIAssetManager.h"
#include "hc/assets/materialDescriptor/hcMaterialDescriptor.h"
#include "hc/assets/model/hcModel.h"

namespace hc::assets::metadata
{
  void ModelMaterialOverride::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, 1);
    writer.writeString(name);
    writer.writeSizeT(materialIndex);
    writer.writeString(sourcePath.toGenericString());
    writer.writeUInt8(sourceType);
    material.serialize(writer);
    writer.finishWritingObject();
  }

  void ModelMaterialOverride::deserialize(io::BinaryReader& reader)
  {
    io::ObjectHeader header = reader.startReadingObject();
    if (!header.match(0, 1))
    {
      reader.finishReadingObject();
      return;
    }

    name = reader.readString();
    materialIndex = reader.readSizeT();
    sourcePath = Path(reader.readString());
    sourceType = static_cast<sourceType::Type>(reader.readUInt8());
    material.deserialize(reader);
    reader.finishReadingObject();
  }

  void ModelMaterialOverride::clear()
  {
    name.clear();
    materialIndex = 0;
    sourcePath.clear();
    sourceType = sourceType::Embedded;
    material.clear();
  }

  static constexpr UInt32 MODEL_METADATA_VERSION = 1;

  void ModelMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, MODEL_METADATA_VERSION);

    Vector<SharedPtr<ModelMaterialOverride>> validMaterialOverrides;
    for (const auto& materialOverride : materialOverrides)
    {
      if (materialOverride)
        validMaterialOverrides.push_back(materialOverride);
    }

    writer.writeSizeT(validMaterialOverrides.size());
    for (const auto& matOverride : validMaterialOverrides)
      matOverride->serialize(writer);
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

    // Material overrides deserialization
    SizeT matOverridesCount = reader.readSizeT();
    materialOverrides.resize(matOverridesCount);
    for (SizeT i = 0; i < matOverridesCount; ++i)
    {
      SharedPtr<ModelMaterialOverride> matOverride = MakeShared<ModelMaterialOverride>();
      matOverride->deserialize(reader);
      materialOverrides[i] = matOverride;
    }
    reader.finishReadingObject();
  }

  void ModelMetadata::addEmbeddedMaterialOverride(
    const String& name,
    SizeT materialIndex,
    const MaterialDescriptor& descriptor
  )
  {
    SharedPtr<ModelMaterialOverride> matOverride = MakeShared<ModelMaterialOverride>();
    matOverride->name = name;
    matOverride->materialIndex = materialIndex;
    matOverride->sourceType = sourceType::Embedded;
    matOverride->material = descriptor;
    materialOverrides.push_back(matOverride);
  }

  void ModelMetadata::addExternalMaterialOverride(
    const String& name,
    SizeT materialIndex,
    const Path& sourcePath
  )
  {
    SharedPtr<ModelMaterialOverride> matOverride = MakeShared<ModelMaterialOverride>();
    matOverride->name = name;
    matOverride->materialIndex = materialIndex;
    matOverride->sourceType = sourceType::External;
    matOverride->sourcePath = sourcePath;
    materialOverrides.push_back(matOverride);
  }

  void ModelMetadata::removeMaterialOverride(const String& name)
  {
    materialOverrides.erase(
      std::remove_if(
        materialOverrides.begin(),
        materialOverrides.end(),
        [&name](const SharedPtr<ModelMaterialOverride>& matOverride)
        {
          return matOverride && matOverride->name == name;
        }
      ),
      materialOverrides.end()
    );
  }

  void ModelMetadata::clear()
  {
    materialOverrides.clear();
  }

  ModelMetadataManager::ModelMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager)
  {
  }

  ModelMetadataManager::~ModelMetadataManager()
  {
  }

  bool ModelMetadataManager::has(const Path& modelPath) const
  {
    String stringPath = modelPath.toGenericString();
    stringPath += hc::assets::metadata::fileFormat::Model::FILE_EXTENSION;
    Path metadataPath(stringPath);
    return metadataPath.exists();
  }

  ModelMetadata ModelMetadataManager::load(const Path& modelPath)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.exists())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::LoadMetadata: Metadata file does not exist for model: %s",
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

      return metadata;
    }
    catch (const Exception& ex)
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::LoadMetadata: Failed to load metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  void ModelMetadataManager::apply(const Path& modelPath, Model& model)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.exists())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::LoadMetadata: Metadata file does not exist for model: %s",
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

      applyMetadata(metadata, model);
    }
    catch (const Exception& ex)
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::LoadMetadata: Failed to load metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  void ModelMetadataManager::save(const Path& modelPath, const Model& model)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.isCreatable())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::SaveMetadata: Metadata file is not creatable for model: %s",
          modelPath.toGenericString().c_str()
        )
      );
    }

    try
    {
      ModelMetadata metadata;

      // Add additional metadata information from the model if needed
      (void) model; // Unused parameter, but can be used to extract more metadata if necessary

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
          "ModelMetadataManager::SaveMetadata: Failed to save metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  void ModelMetadataManager::save(const Path& modelPath, const ModelMetadata& metadata)
  {
    Path metadataPath = GetMetadataFilePath(modelPath);
    if (!metadataPath.isCreatable())
    {
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::SaveMetadata: Metadata file is not creatable for model: %s",
          modelPath.toGenericString().c_str()
        )
      );
    }
    try
    {
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
          "ModelMetadataManager::SaveMetadata: Failed to save metadata for model: %s. Error: %s",
          modelPath.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  void ModelMetadataManager::saveEmbeddedMaterialOverride(
    const Path& modelPath,
    const String& materialName,
    SizeT materialIndex,
    const MaterialDescriptor& descriptor
  )
  {
    if (!has(modelPath))
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::SaveEmbeddedMaterialOverride: Metadata file does not exist for model: %s",
          modelPath.toGenericString().c_str()
        )
      );

    ModelMetadata modelMeta = load(modelPath);
    modelMeta.addEmbeddedMaterialOverride(materialName, materialIndex, descriptor);
    save(modelPath, modelMeta);
  }

  void ModelMetadataManager::saveExternalMaterialOverride(
    const Path& modelPath,
    const String& materialName,
    SizeT materialIndex,
    const Path& sourcePath
  )
  {
    if (!has(modelPath))
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::SaveExternalMaterialOverride: Metadata file does not exist for model: %s",
          modelPath.toGenericString().c_str()
        )
      );

    ModelMetadata modelMeta = load(modelPath);
    modelMeta.addExternalMaterialOverride(materialName, materialIndex, sourcePath);
    save(modelPath, modelMeta);
  }

  void ModelMetadataManager::removeMaterialOverride(
    const Path& modelPath,
    const String& materialName
  )
  {
    if (!has(modelPath))
      throw RuntimeErrorException(
        String::Format(
          "ModelMetadataManager::RemoveMaterialOverride: Metadata file does not exist for model: %s",
          modelPath.toGenericString().c_str()
        )
      );

    ModelMetadata modelMeta = load(modelPath);
    modelMeta.removeMaterialOverride(materialName);
    save(modelPath, modelMeta);
  }

  Path ModelMetadataManager::GetMetadataFilePath(const Path& modelPath)
  {
    String stringPath = modelPath.toGenericString();
    stringPath += hc::assets::metadata::fileFormat::Model::FILE_EXTENSION;
    return Path(stringPath);
  }

  void ModelMetadataManager::applyMetadata(
    const ModelMetadata& metadata,
    Model& model
  )
  {
    for (const SharedPtr<ModelMaterialOverride>& matOverride : metadata.materialOverrides)
    {
      if (!matOverride)
        continue;

      SharedPtr<MaterialDescriptor> modelMat = model.getMaterial(matOverride->name);
      if (!modelMat)
      {
        LogService::Warning(
          String::Format(
            "ModelMetadataManager::applyMetadata: Material '%s' not found in model '%s'. Skipping.",
            matOverride->name.c_str(),
            model.path.toGenericString().c_str()
          )
        );
        continue;
      }

      sourceType::Type sourceType = matOverride->sourceType;
      if (sourceType == sourceType::Embedded)
      {
        *modelMat = matOverride->material;
      }
      else if (sourceType == sourceType::External)
      {
        try
        {
          Path resolvePath = matOverride->sourcePath;
          if (resolvePath.isRelative())
            resolvePath = resolvePath.toAbsolute(m_assetManager.getRootPath());

          SharedPtr<MaterialDescriptor> externalMat = m_assetManager
            .getMaterialDescriptorAssetManager()
            .load(resolvePath);

          if (!externalMat)
            throw RuntimeErrorException(
              String::Format(
                "ModelMetadataManager::applyMetadata: Failed to load external material '%s' for model '%s'. MaterialDescriptor is null.",
                matOverride->name.c_str(),
                model.path.toGenericString().c_str()
              )
            );

          *modelMat = *externalMat;
        }
        catch (const Exception& ex)
        {
          LogService::Warning(
            String::Format(
              "ModelMetadataManager::applyMetadata: Failed to load external material '%s' for model '%s'. Error: %s",
              matOverride->name.c_str(),
              model.path.toGenericString().c_str(),
              ex.what()
            )
          );
        }
      }
    }
  }
}

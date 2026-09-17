#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"

namespace hc
{
  MaterialDescriptorAssetManager::MaterialDescriptorAssetManager() :
    m_loadedMaterialDescriptors(),
    m_defaultMaterialDescriptor(MakeShared<MaterialDescriptor>(""))
  {}

  SharedPtr<MaterialDescriptor> MaterialDescriptorAssetManager::load(
    const Path& path
  )
  {
    if (isLoaded(path))
      return get(path);

    if (!path.exists())
      return nullptr;

    io::BinaryReader reader;

    try
    {
      String error;
      if (!reader.prepare(path, error))
        throw RuntimeErrorException(
          String::Format(
            "MaterialDescriptorAssetManager: Failed to prepare BinaryReader for path: %s. Error: %s",
            path.toGenericString().c_str(),
            error.c_str()
          )
        );

      UInt32 magicNumber = reader.readUInt32();
      if (magicNumber != serialization::fileFormat::MaterialDescriptor::MAGIC_NUMBER)
        throw RuntimeErrorException(
          String::Format(
            "MaterialDescriptorAssetManager: File is not a valid material descriptor at path: %s",
            path.toGenericString().c_str()
          )
        );

      SharedPtr<MaterialDescriptor> materialDescriptor = MakeShared<MaterialDescriptor>();
      materialDescriptor->deserialize(reader);
      reader.shutdown();

      m_loadedMaterialDescriptors[path] = materialDescriptor;
      return materialDescriptor;
    }
    catch (const Exception& ex)
    {
      reader.shutdown();
      throw  RuntimeErrorException(
        String::Format(
          "MaterialDescriptorAssetManager: Failed to load material descriptor at path: %s. Error: %s",
          path.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }

  SharedPtr<MaterialDescriptor> MaterialDescriptorAssetManager::get(
    const Path& path
  ) const
  {
    auto it = m_loadedMaterialDescriptors.find(path);
    if (it != m_loadedMaterialDescriptors.end())
      return it->second;

    throw RuntimeErrorException(
      String::Format(
        "Material descriptor not loaded at path: %s",
        path.toGenericString().c_str()
      )
    );
  }

  bool MaterialDescriptorAssetManager::isLoaded(const Path& path) const
  {
    return m_loadedMaterialDescriptors.find(path) != m_loadedMaterialDescriptors.end();
  }

  void MaterialDescriptorAssetManager::clear()
  {
    m_loadedMaterialDescriptors.clear();
  }

  void MaterialDescriptorAssetManager::getAllLoadedAssets(
    Vector<SharedPtr<MaterialDescriptor>>& outAssets
  ) const
  {
    outAssets.clear();
    for (const auto& pair : m_loadedMaterialDescriptors)
      outAssets.push_back(pair.second);
  }

  SizeT MaterialDescriptorAssetManager::size() const
  {
    return m_loadedMaterialDescriptors.size();
  }

  SharedPtr<MaterialDescriptor> MaterialDescriptorAssetManager::getDefault() const
  {
    return m_defaultMaterialDescriptor;
  }

  void MaterialDescriptorAssetManager::save(
    const Path& path,
    const MaterialDescriptor& descriptor
  )
  {
    if (path.empty())
      throw  RuntimeErrorException(
        "MaterialDescriptorAssetManager: Cannot save material descriptor to an empty path."
      );

    if (!path.isCreatable())
      throw RuntimeErrorException(
        String::Format("MaterialDescriptorAssetManager: Cannot create a file at: %s", path.toGenericString().c_str())
      );

    io::BinaryWriter writer;

    try
    {
      String error;
      if (!writer.prepare(path, error))
        throw IOException(
          String::Format(
            "MaterialDescriptorAssetManager: Failed to prepare BinaryWriter for path: %s. Error: %s",
            path.toGenericString().c_str(),
            error.c_str()
          )
        );

      writer.writeUInt32(serialization::fileFormat::MaterialDescriptor::MAGIC_NUMBER);
      descriptor.serialize(writer);
      writer.shutdown();
    }
    catch (const Exception& ex)
    {
      writer.shutdown();
      throw  RuntimeErrorException(
        String::Format(
          "MaterialDescriptorAssetManager: Failed to save material descriptor at path: %s. Error: %s",
          path.toGenericString().c_str(),
          ex.what()
        )
      );
    }
  }
}

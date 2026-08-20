#include "hc/assets/materialDescriptor/hcMaterialDescriptorAssetManager.h"

namespace hc
{
  MaterialDescriptorAssetManager::MaterialDescriptorAssetManager() :
    m_loadedMaterialDescriptors(),
    m_defaultMaterialDescriptor(nullptr)
  {
    m_defaultMaterialDescriptor = MakeShared<UnlitMaterialDescriptor>(
      "",
      "Default Unlit Material",
      Color(0.5f, 0.5f, 0.5f, 1.0f),
      ""
    );
  }

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::load(
    const Path& path
  )
  {
    try
    {
      if (isLoaded(path))
        return get(path);

      Json json = Json::loadFromFile(path);
      if (json.isNull())
        return nullptr;

      String shaderTypeStr = json["shaderType"].getString();
      shadingType::Type shaderType = shadingType::fromString(shaderTypeStr);

      SharedPtr<AMaterialDescriptor> loadedMaterialDescriptor;
      switch (shaderType)
      {
      case shadingType::Unlit:
        loadedMaterialDescriptor = deserializeUnlitMaterialDescriptor(path, json);
        break;

      default:
        throw InvalidArgumentException(
          String::Format(
            "Unsupported shader type '%s' in material descriptor at path: %s",
            shaderTypeStr.c_str(),
            path.string().c_str()
          )
        );
      }

      if (!loadedMaterialDescriptor)
      {
        LogService::Error(
          String::Format(
            "Failed to load material descriptor at path: %s",
            path.string().c_str()
          )
        );

        return nullptr;
      }

      m_loadedMaterialDescriptors[path] = loadedMaterialDescriptor;
      return loadedMaterialDescriptor;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Exception occurred while loading material descriptor at path '%s': %s",
          path.string().c_str(),
          e.what()
        )
      );

      return nullptr;
    }
  }

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::get(
    const Path& path
  ) const
  {
    auto it = m_loadedMaterialDescriptors.find(path);
    if (it != m_loadedMaterialDescriptors.end())
      return it->second;

    throw RuntimeErrorException(
      String::Format(
        "Material descriptor not loaded at path: %s",
        path.string().c_str()
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
    Vector<SharedPtr<AMaterialDescriptor>>& outAssets
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

  SharedPtr<AMaterialDescriptor> MaterialDescriptorAssetManager::getDefault() const
  {
    return m_defaultMaterialDescriptor;
  }

  SharedPtr<UnlitMaterialDescriptor> MaterialDescriptorAssetManager::deserializeUnlitMaterialDescriptor(
    const Path& path,
    const Json& json
  ) const
  {
    try
    {
      Color color = jsonParsers::parseColor(json["color"]);
      String mainImagePathStr = json["mainImagePath"].getString();
      Path mainImagePath(mainImagePathStr.c_str());

      return MakeShared<UnlitMaterialDescriptor>(
        path,
        "UnlitMaterial",
        color,
        mainImagePath
      );
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to deserialize UnlitMaterialDescriptor: %s",
          e.what()
        )
      );
      return nullptr;
    }
  }
}

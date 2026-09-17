#include "hc/serialization/scene/hcSceneSerializer.h"

#include "hc/serialization/hcFileFormats.h"
#include "hc/serialization/scene/skybox/hcSkyboxSerializer.h"
#include "hc/scene/hcScene.h"
#include "hc/hcVersion.h"

namespace hc
{
  namespace serialization
  {
    bool SceneSerializer::Serialize(
      const Scene& scene,
      const Path& filePath,
      const IAssetManager& assetManager
    )
    {
      try
      {
        String error;
        io::BinaryWriter writer;
        if (!writer.prepare(filePath, error))
        {
          LogService::Error(
            "Exception during scene serialization: " + error +
            " in file: " + filePath.toString()
          );
          return false;
        }

        SerializeHeader(writer);
        scene.serialize(writer);
        SkyboxSerializer::Serialize(scene.getSceneSkybox(), writer, assetManager);

        return true;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene serialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
        return false;
      }
    }

    bool SceneSerializer::Deserialize(
      Scene& scene,
      const Path& filePath,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    )
    {
      try
      {
        scene.destroy();

        String error;
        io::BinaryReader reader;
        if (!reader.prepare(filePath, error))
        {
          LogService::Error(
            "Failed to prepare binary reader for scene file: " + filePath.toString() + " Error: " + error
          );
          return false;
        }

        VerifyHeader(reader);
        scene.deserialize(reader);
        SkyboxSerializer::Deserialize(scene.getSceneSkybox(), reader, assetManager, graphicsManager);

        return true;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
        return false;
      }
    }

    UniquePtr<Scene> SceneSerializer::Deserialize(
      const Path& filePath,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    )
    {
      try
      {
        UniquePtr<Scene> scene = MakeUnique<Scene>();
        if (Deserialize(*scene, filePath, assetManager, graphicsManager))
          return scene;
        else
          return nullptr;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
        return nullptr;
      }
    }

    void SceneSerializer::SerializeHeader(io::BinaryWriter& writer)
    {
      writer.writeUInt32(fileFormat::Scene::MAGIC_NUMBER);
      writer.writeUInt32(fileFormat::Scene::VERSION);
      writer.writeUInt32(EngineVersion::ToInt());
    }

    void SceneSerializer::VerifyHeader(io::BinaryReader& reader)
    {
      UInt32 magicNumber = reader.readUInt32();
      if (magicNumber != fileFormat::Scene::MAGIC_NUMBER)
      {
        throw RuntimeErrorException(
          String::Format("Invalid scene file format: expected magic number %u, got %u",
            fileFormat::Scene::MAGIC_NUMBER,
            magicNumber
          )
        );
      }

      UInt32 version = reader.readUInt32();
      if (version != fileFormat::Scene::VERSION)
      {
        throw RuntimeErrorException(
          String::Format("Unsupported scene file version: expected %u, got %u",
            fileFormat::Scene::VERSION,
            version
          )
        );
      }

      UInt32 engineVersion = reader.readUInt32();
      if (engineVersion > EngineVersion::ToInt())
      {
        LogService::Warning(
          "Scene file was created with a newer engine version: " +
          std::to_string(engineVersion) + ". Some features may not be supported."
        );
      }
    }
  }
}

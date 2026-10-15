#include "hc/serialization/scene/hcSceneSerializer.h"

#include "hc/serialization/hcFileFormats.h"
#include "hc/scene/hcScene.h"
#include "hc/hcVersion.h"

namespace hc
{
  namespace serialization
  {
    void SceneSerializer::Serialize(
      const Scene& scene,
      const Path& filePath
    )
    {
      try
      {
        String error;
        io::BinaryWriter writer;
        if (!writer.prepare(filePath, error))
        {
          throw RuntimeErrorException(
            "SceneSerializer::Serialize: Error during scene serialization: " + error +
            " in file: " + filePath.toString()
          );
        }

        SerializeHeader(writer);
        scene.serialize(writer);
      }
      catch (const std::exception& e)
      {
        throw RuntimeErrorException(
          "SceneSerializer::Serialize: Error during scene serialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
      }
    }

    void SceneSerializer::Deserialize(
      Scene& scene,
      const Path& filePath,
      IAssetManager& assetManager,
      IGraphicsManager& graphicsManager
    )
    {
      try
      {
        String error;
        io::BinaryReader reader;
        if (!reader.prepare(filePath, error))
        {
          throw RuntimeErrorException(
            "SceneSerializer::Deserialize: Failed to prepare binary reader for scene file: " + filePath.toString() + " Error: " + error
          );
        }

        VerifyHeader(reader);
        scene.destroy();
        scene.deserialize(reader);
      }
      catch (const Exception& e)
      {
        throw RuntimeErrorException(
          "SceneSerializer::Deserialize: Error during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
      }

      try
      {
        if (scene.getSettings().skyboxSettings.sourcePath.empty() != true)
        {
          scene.getSceneSkybox().initialize(
            assetManager,
            graphicsManager,
            scene.getSettings().skyboxSettings
          );
        }
      }
      catch (const Exception& e)
      {
        throw RuntimeErrorException(
          "SceneSerializer::Deserialize: Error during skybox initialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
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
        Deserialize(*scene, filePath, assetManager, graphicsManager);
        return scene;
      }
      catch (const std::exception& e)
      {
        throw RuntimeErrorException(
          "Exception during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.toString()
        );
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
          String::Format("SceneSerializer: Invalid scene file format: expected magic number %u, got %u",
            fileFormat::Scene::MAGIC_NUMBER,
            magicNumber
          )
        );
      }

      UInt32 version = reader.readUInt32();
      if (version != fileFormat::Scene::VERSION)
      {
        throw RuntimeErrorException(
          String::Format("SceneSerializer: Unsupported scene file version: expected %u, got %u",
            fileFormat::Scene::VERSION,
            version
          )
        );
      }

      UInt32 engineVersion = reader.readUInt32();
      if (engineVersion > EngineVersion::ToInt())
      {
        LogService::Warning(
          "SceneSerializer: Scene file was created with a newer engine version: " +
          std::to_string(engineVersion) + ". Some features may not be supported."
        );
      }
    }
  }
}

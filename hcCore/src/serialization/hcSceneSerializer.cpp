#include "hc/serialization/hcSceneSerializer.h"
#include <fstream>
#include "hc/serialization/hcFileFormats.h"
#include "hc/scene/hcScene.h"
#include "hc/hcVersion.h"

namespace hc
{
  namespace serialization
  {
    bool SceneSerializer::Serialize(
      const Scene& scene,
      const Path& filePath
    )
    {
      try
      {
        // Open the file for binary writing
        std::ofstream outputFile(filePath, std::ios::binary);
        if (!outputFile)
        {
          LogService::Error(
            "Failed to open file for writing: " + filePath.string()
          );
          return false;
        }

        BinaryWriter writer(outputFile);

        SerializeHeader(writer);
        scene.serialize(writer);
        return true;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene serialization: " + String(e.what()) +
          " in file: " + filePath.string()
        );
        return false;
      }
    }

    bool SceneSerializer::Deserialize(Scene& scene, const Path& filePath)
    {
      try
      {
        // Open the file for binary reading
        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile)
        {
          LogService::Error(
            "Failed to open file for reading: " + filePath.string()
          );
          return false;
        }

        BinaryReader reader(inputFile);

        VerifyHeader(reader);
        scene.deserialize(reader);
        return true;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.string()
        );
        return false;
      }
    }

    UniquePtr<Scene> SceneSerializer::Deserialize(const Path& filePath)
    {
      try
      {
        UniquePtr<Scene> scene = MakeUnique<Scene>();
        if (Deserialize(*scene, filePath))
          return scene;
        else
          return nullptr;
      }
      catch (const std::exception& e)
      {
        LogService::Error(
          "Exception during scene deserialization: " + String(e.what()) +
          " in file: " + filePath.string()
        );
        return nullptr;
      }
    }

    void SceneSerializer::SerializeHeader(BinaryWriter& writer)
    {
      writer.writeUInt32(fileFormat::Scene::MAGIC_NUMBER);
      writer.writeUInt32(fileFormat::Scene::VERSION);
      writer.writeUInt32(EngineVersion::ToInt());
    }

    void SceneSerializer::VerifyHeader(BinaryReader& reader)
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

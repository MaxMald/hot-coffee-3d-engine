#include "hc/editor/serialization/hcProjectSerializer.h"
#include <fstream>
#include "hc/editor/services/projectManager/hcProject.h"

namespace hc::editor::serialization
{
  UniquePtr<Project> ProjectSerializer::Deserialize(const Path& filePath)
  {
    try
    {
      UniquePtr<Project> project = MakeUnique<Project>();

      std::ifstream fileStream(filePath, std::ios::binary);
      if (!fileStream.is_open())
      {
        LogService::Error(
          "Failed to open project file for reading: " + filePath.string()
        );
        return nullptr;
      }

      BinaryReader reader(fileStream);
      project->deserialize(reader);
      return project;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to deserialize project file: " + filePath.string() + " Error: " + e.what()
      );
      return nullptr;
    }
  }
  bool ProjectSerializer::Serialize(const Project& project, const Path& filePath)
  {
    try
    {
      std::ofstream fileStream(filePath, std::ios::binary);
      if (!fileStream.is_open())
      {
        LogService::Error(
          "Failed to open project file for writing: " + filePath.string()
        );
        return false;
      }        

      BinaryWriter writer(fileStream);
      project.serialize(writer);
      return true;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to serialize project file: " + filePath.string() + " Error: " + e.what()
      );
      return false;
    }
  }
}

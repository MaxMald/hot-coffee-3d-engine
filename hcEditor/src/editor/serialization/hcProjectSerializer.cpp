#include "hc/editor/serialization/hcProjectSerializer.h"
#include "hc/editor/services/projectManager/hcProject.h"

namespace hc::editor::serialization
{
  UniquePtr<Project> ProjectSerializer::Deserialize(const Path& filePath)
  {
    try
    {
      UniquePtr<Project> project = MakeUnique<Project>();

      String error;
      io::BinaryReader reader;
      if (!reader.prepare(filePath, error))
      {
        LogService::Error(
          "Failed to prepare binary reader for project file: " + filePath.toString() + " Error: " + error
        );
        return nullptr;
      }

      project->deserialize(reader);
      return project;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to deserialize project file: " + filePath.toString() + " Error: " + e.what()
      );
      return nullptr;
    }
  }
  bool ProjectSerializer::Serialize(const Project& project, const Path& filePath)
  {
    try
    {
      String error;
      io::BinaryWriter writer;
      if (!writer.prepare(filePath, error))
      {
        LogService::Error(
          "Failed to prepare binary writer for project file: " + filePath.toString() + " Error: " + error
        );
        return false;
      }

      project.serialize(writer);
      return true;
    }
    catch (const Exception& e)
    {
      LogService::Error(
        "Failed to serialize project file: " + filePath.toString() + " Error: " + e.what()
      );
      return false;
    }
  }
}

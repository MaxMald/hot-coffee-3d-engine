#include "hc/editor/metadata/hcEditorMetadataStructures.h"
#include "hc/editor/metadata/hcEditorMetadataCommons.h"

namespace hc::editor::metadata
{
  // -------------------- EditorMetadata Implementation --------------------

  static constexpr UInt32 EDITOR_METADATA_VERSION = 1;

  void EditorMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(fileFormat::EditorMetadata::MAGIC_NUMBER, EDITOR_METADATA_VERSION);

    writer.writeSizeT(lastOpenedProjects.size());
    for (const Path& projectPath : lastOpenedProjects)
      writer.writePath(projectPath);

    writer.finishWritingObject();
  }

  void EditorMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchType(fileFormat::EditorMetadata::MAGIC_NUMBER))
    {
      LogService::Error("EditorMetadata::deserialize - invalid object.");
      reader.finishReadingObject();
      return;
    }

    if (!header.matchVersion(EDITOR_METADATA_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    SizeT numProjects = reader.readSizeT();
    for (SizeT i = 0; i < numProjects; ++i)
    {
      Path projectPath = reader.readPath();
      lastOpenedProjects.push_back(projectPath);
    }
    reader.finishReadingObject();
  }

  void EditorMetadata::clear()
  {
    lastOpenedProjects.clear();
  }

  // -------------------- ProjectMetadata Implementation --------------------

  static constexpr UInt32 PROJECT_METADATA_VERSION = 1;

  void ProjectMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(fileFormat::ProjectMetadata::MAGIC_NUMBER, PROJECT_METADATA_VERSION);

    writer.writeSizeT(lastOpenedScenes.size());
    for (const Path& scenePath : lastOpenedScenes)
      writer.writePath(scenePath);

    writer.finishWritingObject();
  }

  void ProjectMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchType(fileFormat::ProjectMetadata::MAGIC_NUMBER))
    {
      LogService::Error("ProjectMetadata::deserialize - invalid object.");
      reader.finishReadingObject();
      return;
    }

    if (!header.matchVersion(PROJECT_METADATA_VERSION))
    {
      reader.finishReadingObject();
      return;
    }

    SizeT numScenes = reader.readSizeT();
    for (SizeT i = 0; i < numScenes; ++i)
    {
      Path scenePath = reader.readPath();
      lastOpenedScenes.push_back(scenePath);
    }

    reader.finishReadingObject();
  }

  void ProjectMetadata::clear()
  {
    lastOpenedScenes.clear();
  }
}

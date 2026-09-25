#include "hc/editor/metadata/hcEditorMetadataStructures.h"

namespace hc::editor::metadata
{
  // -------------------- EditorMetadata Implementation --------------------

  static constexpr UInt32 EDITOR_METADATA_VERSION = 1;

  void EditorMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, EDITOR_METADATA_VERSION);

    writer.writeSizeT(lastOpenedProjects.size());
    for (const Path& projectPath : lastOpenedProjects)
      writer.writePath(projectPath);

    writer.finishWritingObject();
  }

  void EditorMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
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
    writer.startWritingObject(0, PROJECT_METADATA_VERSION);

    writer.writeSizeT(lastOpenedScenes.size());
    for (const Path& scenePath : lastOpenedScenes)
      writer.writePath(scenePath);

    writer.finishWritingObject();
  }

  void ProjectMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
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

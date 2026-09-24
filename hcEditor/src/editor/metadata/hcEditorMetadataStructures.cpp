#include "hc/editor/metadata/hcEditorMetadataStructures.h"

namespace hc::editor::metadata
{
  // -------------------- EditorMetadata Implementation --------------------

  static constexpr UInt32 METADATA_VERSION = 1;

  void EditorMetadata::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, METADATA_VERSION);

    writer.writeSizeT(lastOpenedProjects.size());
    for (const Path& projectPath : lastOpenedProjects)
      writer.writePath(projectPath);

    writer.finishWritingObject();
  }

  void EditorMetadata::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    if (!header.matchVersion(METADATA_VERSION))
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
}

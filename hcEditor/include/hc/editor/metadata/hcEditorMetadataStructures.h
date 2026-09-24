#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor::metadata
{
  /**
   * @brief Represents metadata for the editor.
   */
  struct EditorMetadata : public hc::io::ISerializable
  {
    Vector<Path> lastOpenedProjects;  ///< List of paths to the last opened projects in the editor

    void serialize(io::BinaryWriter& writer) const override;
    void deserialize(io::BinaryReader& reader) override;
    void clear();
  };
}

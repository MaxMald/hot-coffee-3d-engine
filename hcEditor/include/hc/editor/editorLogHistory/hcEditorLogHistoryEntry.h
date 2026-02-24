#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistoryEntryType.h"

namespace hc::editor
{
  struct EditorLogHistoryEntry
  {
    String message = "";
    editorLogHistoryEntryType::Type type = editorLogHistoryEntryType::Message;
  };
}

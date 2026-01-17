#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLoggerEntryType.h"

namespace hc::editor
{
  struct EditorLoggerEntry
  {
    String message = "";
    editorLoggerEntryType::Type type = editorLoggerEntryType::Message;
  };
}

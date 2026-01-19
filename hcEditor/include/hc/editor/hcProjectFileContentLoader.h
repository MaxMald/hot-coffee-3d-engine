#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcProjectFileContent.h"

namespace hc
{
  class Json;
}

namespace hc::editor
{
  namespace projectFileContentLoader
  {
    Optional<ProjectFileContent> loadFromJson(const Json& json);
  }
}

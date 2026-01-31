#include "hc/editor/hcProjectFileContentLoader.h"

namespace hc::editor
{
  namespace projectFileContentLoader
  {
    Optional<ProjectFileContent> loadFromJson(const Json& json)
    {
      ProjectFileContent content{};

      if (json.contains("projectConfiguration"))
      {
        const Json projectConfigJson = json["projectConfiguration"];
        if (projectConfigJson.isObject())
        {
          if (projectConfigJson.contains("majorVersion"))
            content.projectConfiguration.majorVersion = static_cast<UInt32>(projectConfigJson["majorVersion"].getInt32());
          else
            content.projectConfiguration.majorVersion = 0;

          if (projectConfigJson.contains("minorVersion"))
            content.projectConfiguration.minorVersion = static_cast<UInt32>(projectConfigJson["minorVersion"].getInt32());
          else
            content.projectConfiguration.minorVersion = 0;

          if (projectConfigJson.contains("patchVersion"))
            content.projectConfiguration.patchVersion = static_cast<UInt32>(projectConfigJson["patchVersion"].getInt32());
          else
            content.projectConfiguration.patchVersion = 0;
        }
      }

      return content;
    }
  }
}

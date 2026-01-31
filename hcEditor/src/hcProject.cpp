#include "hc/editor/hcProject.h"

#include "hc/editor/hcProjectFileContentLoader.h"

namespace hc::editor
{
  Project::Project() :
    m_projectFileContent()
  {
  }

  Project::~Project()
  {
  }

  bool Project::loadFromFile(const Path& filePath)
  {
    Json json = Json::loadFromFile(filePath);
    if (json.isNull())
      return false;

    auto optionalContent = projectFileContentLoader::loadFromJson(json);
    if (!optionalContent.has_value())
      return false;

    m_projectFileContent = optionalContent.value();
    return true;
  }

  bool Project::saveToFile(const Path& filePath) const
  {
    return false;
  }

  ProjectFileContent& Project::getProjectFileContent()
  {
    return m_projectFileContent;
  }

  const ProjectFileContent& Project::getProjectFileContent() const
  {
    return m_projectFileContent;
  }
}

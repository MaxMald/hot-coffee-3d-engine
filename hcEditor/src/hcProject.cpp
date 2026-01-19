#include "hc/editor/hcProject.h"

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
  }

  bool Project::saveToFile(const Path& filePath) const
  {
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

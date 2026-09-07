#include "hc/editor/services/projectManager/hcProject.h"
#include "hc/editor/serialization/hcFileFormats.h"

namespace hc::editor
{
  Project* Project::CreateEmpty()
  {
    Project* newProject = new Project();
    newProject->m_majorVersion = hc::editor::serialization::fileFormats::Project::MAJOR_VERSION;
    newProject->m_minorVersion = hc::editor::serialization::fileFormats::Project::MINOR_VERSION;
    newProject->m_patchVersion = hc::editor::serialization::fileFormats::Project::PATCH_VERSION;
    return newProject;
  }

  Project::Project() :
    m_majorVersion(0),
    m_minorVersion(0),
    m_patchVersion(0),
    m_projectFilePath(),
    m_relativePathToLastOpenedScene()
  {
  }

  Project::~Project()
  {
  }

  void Project::serialize(io::BinaryWriter& writer) const
  {
    writer.writeUInt32(hc::editor::serialization::fileFormats::Project::MAGIC);
    writer.writeUInt16(m_majorVersion);
    writer.writeUInt16(m_minorVersion);
    writer.writeUInt16(m_patchVersion);
    writer.writeString(m_relativePathToLastOpenedScene);
  }

  void Project::deserialize(io::BinaryReader& reader)
  {
    m_projectFilePath.clear();

    UInt32 magic = reader.readUInt32();
    if (magic != hc::editor::serialization::fileFormats::Project::MAGIC)
      throw InvalidArgumentException("Invalid project file format.");

    m_majorVersion = reader.readUInt16();
    m_minorVersion = reader.readUInt16();
    m_patchVersion = reader.readUInt16();
    m_relativePathToLastOpenedScene = reader.readString();
  }

  void Project::setProjectFilePath(const Path& path)
  {
    m_projectFilePath = path;
  }

  const Path& Project::getProjectFilePath() const
  {
    return m_projectFilePath;
  }

  void Project::setPathToLastOpenedScene(const Path& absolutePath)
  {
    if (m_projectFilePath.empty())
      throw RuntimeErrorException(
        "Project file path must be set before setting the last opened scene path."
      );

    m_relativePathToLastOpenedScene = AssetPath::ToRelative(
      absolutePath,
      m_projectFilePath.parent_path()
    );
  }

  const String& Project::getPathToLastOpenedScene() const
  {
    return m_relativePathToLastOpenedScene;
  }
}

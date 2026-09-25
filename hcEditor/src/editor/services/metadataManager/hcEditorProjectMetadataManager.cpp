#include "hc/editor/services/metadataManager/hcEditorProjectMetadataManager.h"

#include "hc/editor/metadata/hcEditorMetadataCommons.h"

namespace hc::editor
{
  static constexpr SizeT MAX_LAST_OPENED_SCENES = 10;

  void EditorProjectMetadataManager::saveLastOpenedScenePath(
    const Path& projectFilepath,
    const Path& scenePath
  )
  {
    if (scenePath.empty())
      return;

    try
    {
      Path projectDirectory = projectFilepath.parentPath();
      Path relativePath = scenePath.toRelative(projectDirectory);

      Int32 size = m_projectMetadata.lastOpenedScenes.size();
      for (Int32 i = size - 1; i >= 0; --i)
      {
        if (m_projectMetadata.lastOpenedScenes[i] == relativePath)
          m_projectMetadata.lastOpenedScenes.erase(m_projectMetadata.lastOpenedScenes.begin() + i);
      }

      m_projectMetadata.lastOpenedScenes.insert(
        m_projectMetadata.lastOpenedScenes.begin(),
        relativePath
      );

      if (m_projectMetadata.lastOpenedScenes.size() > MAX_LAST_OPENED_SCENES)
        m_projectMetadata.lastOpenedScenes.resize(MAX_LAST_OPENED_SCENES);

      saveProjectMetadata(projectFilepath);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "Failed to save last opened scene: '%s' for project '%s'. Error: %s",
          scenePath.toString().c_str(),
          projectFilepath.toString().c_str(),
          e.what()
        )
      );
      return;
    }
  }

  void EditorProjectMetadataManager::loadProjectMetadata(const Path& projectFilepath)
  {
    clearProjectMetadata();

    if (!projectFilepath.exists())
      return;

    String projectFilename = projectFilepath.filename().toString();
    String metaFilename = projectFilename + String(metadata::fileFormat::ProjectMetadata::FILE_EXTENSION);

    Path metaFilePath = projectFilepath.parentPath() / metaFilename;
    if (!metaFilePath.exists())
      return;

    io::BinaryReader reader;

    try
    {
      String error;
      if (!reader.prepare(metaFilePath, error))
      {
        LogService::Error(
          String::Format(
            "An error occurred while preparing to read project metadata file '%s': %s",
            metaFilePath.toString().c_str(),
            error.c_str()
          )
        );
        return;
      }

      m_projectMetadata.deserialize(reader);
      reader.shutdown();
    }
    catch (const Exception& e)
    {
      reader.shutdown();
      LogService::Error(
        String::Format(
          "An error occurred while reading project metadata file '%s': %s",
          metaFilePath.toString().c_str(),
          e.what()
        )
      );
    }
  }

  void EditorProjectMetadataManager::saveProjectMetadata(const Path& projectFilepath) const
  {
    if (projectFilepath.empty() || !projectFilepath.exists())
      return;

    String projectFilename = projectFilepath.filename().toString();
    String metaFilename = projectFilename + String(metadata::fileFormat::ProjectMetadata::FILE_EXTENSION);
    Path metaFilePath = projectFilepath.parentPath() / metaFilename;

    io::BinaryWriter writer;

    try
    {
      String error;
      if (!writer.prepare(metaFilePath, error))
      {
        LogService::Error(
          String::Format(
            "An error occurred while preparing to write project metadata file '%s': %s",
            metaFilePath.toString().c_str(),
            error.c_str()
          )
        );
        return;
      }

      m_projectMetadata.serialize(writer);
      writer.shutdown();
    }
    catch (const Exception& e)
    {
      writer.shutdown();
      LogService::Error(
        String::Format(
          "An error occurred while writing project metadata file '%s': %s",
          metaFilePath.toString().c_str(),
          e.what()
        )
      );
    }
  }

  void EditorProjectMetadataManager::clearProjectMetadata()
  {
    m_projectMetadata.clear();
  }
}

#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

#include <hc/assets/metadata/hcModelMetadataManager.h>

#include "hc/editor/metadata/hcEditorMetadataCommons.h"

namespace hc::editor
{
  static constexpr const char* EDITOR_METADATA_FILE_NAME = "editor_metadata";
  static constexpr SizeT MAX_LAST_OPENED_PROJECTS = 10;

  EditorMetadataManager::EditorMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager),
    m_modelMetadataManager(assetManager),
    m_editorMetadata()
  {}

  EditorMetadataManager::~EditorMetadataManager()
  {}

  void EditorMetadataManager::prepare()
  {
    loadEditorMetadata();
  }

  void EditorMetadataManager::destroy()
  {}

  void EditorMetadataManager::saveEditorMetadata() const
  {
    String metadataFileName = String(EDITOR_METADATA_FILE_NAME)
      + String(metadata::fileFormat::EditorMetadata::FILE_EXTENSION);
    Path metadataFilePath = Path::GetCurrentPath() / metadataFileName;

    if (!metadataFilePath.isCreatable())
    {
      LogService::Error(
        String::Format(
          "Failed to save editor metadata: Path '%s' is not creatable.",
          metadataFilePath.toString().c_str()
        )
      );
      return;
    }

    io::BinaryWriter writer;

    try
    {
      String error;
      if (!writer.prepare(metadataFilePath, error))
      {
        throw IOException(
          String::Format(
            "Failed to prepare BinaryWriter for path '%s': %s",
            metadataFilePath.toString().c_str(),
            error.c_str()
          )
        );
      }

      m_editorMetadata.serialize(writer);
      writer.shutdown();
    }
    catch (const Exception& e)
    {
      writer.shutdown();
      LogService::Error(
        String::Format(
          "Failed to save editor metadata: %s",
          e.what()
        )
      );
    }
  }

  void EditorMetadataManager::saveLastOpenedProjectPath(const Path& projectPath)
  {
    if (projectPath.empty())
      return;

    auto& lastOpenedProjects = m_editorMetadata.lastOpenedProjects;

    // Remove the project path if it already exists to avoid duplicates
    Int32 size = static_cast<Int32>(lastOpenedProjects.size());
    for (Int32 i = size - 1; i >= 0; --i)
    {
      if (lastOpenedProjects[static_cast<SizeT>(i)] == projectPath)
        lastOpenedProjects.erase(lastOpenedProjects.begin() + i);
    }

    // Add the new project path to the front of the list
    lastOpenedProjects.insert(lastOpenedProjects.begin(), projectPath);

    if (lastOpenedProjects.size() > MAX_LAST_OPENED_PROJECTS)
      lastOpenedProjects.resize(MAX_LAST_OPENED_PROJECTS);

    saveEditorMetadata();
  }

  void EditorMetadataManager::loadEditorMetadata()
  {
    String metadataFileName = String(EDITOR_METADATA_FILE_NAME)
      + String(metadata::fileFormat::EditorMetadata::FILE_EXTENSION);
    Path metadataFilePath = Path::GetCurrentPath() / metadataFileName;

    if (!metadataFilePath.exists())
      return;

    io::BinaryReader reader;

    try
    {
      String error;
      if (!reader.prepare(metadataFilePath, error))
      {
        throw IOException(
          String::Format(
            "Failed to prepare BinaryReader for path '%s': %s",
            metadataFilePath.toString().c_str(),
            error.c_str()
          )
        );
      }

      m_editorMetadata.deserialize(reader);
      reader.shutdown();
    }
    catch (const Exception& e)
    {
      reader.shutdown();
      LogService::Error(
        String::Format(
          "Failed to load editor metadata: %s",
          e.what()
        )
      );
    }

    Vector<Path> validProjectPaths;
    for (const Path& projectPath : m_editorMetadata.lastOpenedProjects)
    {
      if (projectPath.exists())
        validProjectPaths.push_back(projectPath);
    }

    m_editorMetadata.lastOpenedProjects = validProjectPaths;
  }
}

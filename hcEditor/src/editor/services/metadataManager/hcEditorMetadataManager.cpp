#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

#include <hc/assets/metadata/hcModelMetadataManager.h>

#include "hc/editor/metadata/hcEditorMetadataCommons.h"

namespace hc::editor
{
  static constexpr const char* EDITOR_METADATA_FILE_NAME = "editor_metadata";

  EditorMetadataManager::EditorMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager),
    m_modelMetadataManager(assetManager),
    m_editorMetadata()
  {}

  EditorMetadataManager::~EditorMetadataManager()
  {}

  void EditorMetadataManager::prepare()
  {}

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

  void EditorMetadataManager::loadEditorMetadata()
  {
    String metadataFileName = String(EDITOR_METADATA_FILE_NAME)
      + String(metadata::fileFormat::EditorMetadata::FILE_EXTENSION);
    Path metadataFilePath = Path::GetCurrentPath() / metadataFileName;

    if (!metadataFilePath.exists())
      return;
  }
}

#include "hc/editor/services/metadataManager/hcEditorMetadataManager.h"

#include <hc/assets/metadata/hcModelMetadataManager.h>

namespace hc::editor
{
  EditorMetadataManager::EditorMetadataManager(IAssetManager& assetManager) :
    m_assetManager(assetManager),
    m_modelMetadataManager(assetManager)
  {}

  EditorMetadataManager::~EditorMetadataManager()
  {}

  void EditorMetadataManager::prepare()
  {}

  void EditorMetadataManager::destroy()
  {}
}

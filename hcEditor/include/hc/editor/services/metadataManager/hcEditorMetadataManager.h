#pragma once

#include "hc/editor/metadata/hcEditorMetadataStructures.h"
#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/metadataManager/hcEditorModelMetadataManager.h"

namespace hc::editor
{
  class EditorMetadataManager : public IEditorService
  {
  public:
    EditorMetadataManager(IAssetManager& assetManager);
    virtual ~EditorMetadataManager() override;

    void prepare() override;
    void destroy() override;

    /**
     * @brief Retrieves the model metadata manager.
     *
     * @return Reference to the EditorModelMetadataManager instance.
     */
    inline EditorModelMetadataManager& getModelMetadataManager()
    {
      return m_modelMetadataManager;
    }

    /**
     * @brief Retrieves the editor metadata.
     *
     * @return Reference to the EditorMetadata instance.
     */
    inline metadata::EditorMetadata& getEditorMetadata()
    {
      return m_editorMetadata;
    }

    /**
     * @brief Retrieves the editor metadata (const version).
     *
     * @return Const reference to the EditorMetadata instance.
     */
    inline const metadata::EditorMetadata& getEditorMetadata() const
    {
      return m_editorMetadata;
    }

    /**
     * @brief Saves the editor metadata to the default file path.
     */
    void saveEditorMetadata() const;

    /**
     * @brief Loads the editor metadata from the default file path.
     */
    void loadEditorMetadata();

  private:
    IAssetManager& m_assetManager;
    EditorModelMetadataManager m_modelMetadataManager;
    metadata::EditorMetadata m_editorMetadata;
  };
}

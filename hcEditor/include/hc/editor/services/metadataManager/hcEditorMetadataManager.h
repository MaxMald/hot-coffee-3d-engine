#pragma once

#include "hc/editor/metadata/hcEditorMetadataStructures.h"
#include "hc/editor/services/hcIEditorService.h"
#include "hc/editor/services/metadataManager/hcEditorModelMetadataManager.h"
#include "hc/editor/services/metadataManager/hcEditorProjectMetadataManager.h"

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
     * @return Reference to the EditorModelMetadataManager instance.
     */
    inline EditorModelMetadataManager& getModelMetadataManager()
    {
      return m_modelMetadataManager;
    }

    /**
     * @brief Retrieves the project metadata manager.
     * @return Reference to the EditorProjectMetadataManager instance.
     */
    inline EditorProjectMetadataManager& getProjectMetadataManager()
    {
      return m_projectMetadataManager;
    }

    /**
     * @brief Saves the editor metadata to the default file path.
     */
    void saveEditorMetadata() const;

    /**
     * @brief Saves the path of the last opened project to the editor metadata.
     * @param projectPath The path of the last opened project.
     */
    void saveLastOpenedProjectPath(const Path& projectPath);

    /**
     * @brief Retrieves the paths of the last opened projects from the editor metadata.
     */
    inline const Vector<Path>& getLastOpenedProjectPaths() const
    {
      return m_editorMetadata.lastOpenedProjects;
    }

    /**
     * @brief Loads the editor metadata from the default file path.
     */
    void loadEditorMetadata();

  private:
    IAssetManager& m_assetManager;
    EditorModelMetadataManager m_modelMetadataManager;
    EditorProjectMetadataManager m_projectMetadataManager;
    metadata::EditorMetadata m_editorMetadata;
  };
}

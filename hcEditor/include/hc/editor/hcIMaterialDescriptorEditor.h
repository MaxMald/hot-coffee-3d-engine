#pragma once

#include <hc/hcNonCopyable.h>
#include <hc/hcMaterialDescriptor.h>
#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcAssetFileReference.h"

namespace hc::editor
{
  class ProjectFileSelectorView;

  /**
   * @brief Interface for material descriptor editors.
   */
  class IMaterialDescriptorEditor :
    public NonCopyable
  {
  public:
    virtual ~IMaterialDescriptorEditor() = default;

    /**
     * @brief Initializes the editor with a project file selector view.
     *
     * @param projectFileSelector Shared pointer to the project file selector
     * view.
     */
    virtual void init(SharedPtr<ProjectFileSelectorView> projectFileSelector) = 0;

    /**
     * @brief Copies values from the given material descriptor asset.
     * 
     * @param asset Shared pointer to the material descriptor asset.
     */
    virtual void copyValuesFrom(SharedPtr<MaterialDescriptor> asset) = 0;

    /**
     * @brief Draws the editor UI for the material descriptor.
     */
    virtual void draw() = 0;

    /**
     * @brief Saves the current material descriptor to the specified path.
     * 
     * @param path Target file path for saving.
     */
    virtual void save(const Path& path) = 0;

    /**
     * @brief Clears the editor state and resets all values.
     */
    virtual void clear() = 0;
  };
}

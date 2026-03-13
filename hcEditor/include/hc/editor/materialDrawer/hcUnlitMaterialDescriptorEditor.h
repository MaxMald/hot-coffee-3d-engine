#pragma once

#include "hc/editor/materialDescriptorEditor/hcIMaterialDescriptorEditor.h"

namespace hc::editor
{
  class ProjectFileSelectorView;

  /**
   * @brief Editor for unlit material descriptors.
   */
  class UnlitMaterialDescriptorEditor :
    public IMaterialDescriptorEditor
  {
  public:
    UnlitMaterialDescriptorEditor(ProjectFileSelectorView& projectFileSelector);
    virtual ~UnlitMaterialDescriptorEditor();

    /**
     * @copydoc IMaterialDescriptorEditor::init
     */
    void init() override;

    /**
     * @copydoc IMaterialDescriptorEditor::copyValuesFrom
     */
    void copyValuesFrom(SharedPtr<AMaterialDescriptor> asset) override;

    /**
     * @copydoc IMaterialDescriptorEditor::draw
     */
    void draw() override;

    /**
     * @copydoc IMaterialDescriptorEditor::save
     */
    void save(const Path& path) override;

    /**
     * @copydoc IMaterialDescriptorEditor::clear
     */
    void clear() override;

  private:
    ProjectFileSelectorView& m_projectFileSelector;
    Color m_color;
    Path m_mainImagePath;
  };
}

#pragma once

#include "hc/editor/hcIMaterialDescriptorEditor.h"

namespace hc::editor
{
  /**
   * @brief Editor for unlit material descriptors.
   */
  class UnlitMaterialDescriptorEditor :
    public IMaterialDescriptorEditor
  {
  public:
    UnlitMaterialDescriptorEditor();
    virtual ~UnlitMaterialDescriptorEditor();

    /**
     * @copydoc IMaterialDescriptorEditor::init
     */
    void init() override;

    /**
     * @copydoc IMaterialDescriptorEditor::copyValuesFrom
     */
    void copyValuesFrom(SharedPtr<MaterialDescriptor> asset) override;

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
    Color m_color;
    Path m_mainImagePath;
  };
}

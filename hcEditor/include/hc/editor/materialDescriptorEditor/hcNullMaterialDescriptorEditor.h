#pragma once

#include "hc/editor/materialDescriptorEditor/hcIMaterialDescriptorEditor.h"

namespace hc::editor
{
  /**
   * @brief A no-op material descriptor editor implementation.
   */
  class NullMaterialDescriptorEditor :
    public IMaterialDescriptorEditor
  {
  public:
    NullMaterialDescriptorEditor() = default;
    virtual ~NullMaterialDescriptorEditor() = default;

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
  };
}

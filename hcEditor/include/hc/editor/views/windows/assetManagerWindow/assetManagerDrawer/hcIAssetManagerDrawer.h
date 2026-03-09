#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * Interface for asset manager drawer components.
   *
   * Defines the contract for drawing asset management UI components
   * within the asset manager window.
   */
  class IAssetManagerDrawer : public NonCopyable
  {
  public:
    /**
     * Virtual destructor.
     */
    virtual ~IAssetManagerDrawer() = default;

    /**
     * Draws the asset manager UI component.
     */
    virtual void draw() = 0;

  protected:
    /**
     * Default constructor.
     */
    IAssetManagerDrawer() = default;
  };
}

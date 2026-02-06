#pragma once

#include "hc/editor/hcIMenuItem.h"

namespace hc::editor
{
  class ProjectManager;

  /**
   * @brief Menu item that opens a project selection dialog.
   */
  class OpenProjectMenuItem : public IMenuItem
  {
  public:
    /**
     * @brief Constructs an OpenProjectMenuItem with the given project manager.
     *
     * @param projectManager Reference to the ProjectManager used to open
     * projects.
     */
    OpenProjectMenuItem(ProjectManager& projectManager);
    ~OpenProjectMenuItem() override = default;

    /**
     * @brief Draws the menu item and handles the open project dialog.
     */
    void draw() override;

  private:
    ProjectManager& m_projectManager;

    bool shouldDrawDialog() const;
    void drawDialog();
  };
}

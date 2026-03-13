#pragma once

namespace hc::editor
{
  class ProjectManager;

  /**
   * @brief Interface for listening to project manager events.
   */
  class IProjectManagerListener
  {
  public:
    virtual ~IProjectManagerListener() = default;

  protected:
    /**
     * @brief Called when a project is opened.
     */
    virtual void onProjectOpened() = 0;

    /**
     * @brief Called when a project is closed.
     */
    virtual void onProjectClosed() = 0;

    friend class ProjectManager;
  };
}

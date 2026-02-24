#pragma once

#include "hc/editor/views/hcIView.h"
#include "hc/editor/views/fileDialog/hcFileDialogRequest.h"

namespace hc::editor
{
  /**
   * @brief View responsible for managing and displaying file dialogs in the
   * editor.
   *
   * Handles the lifecycle of file dialog requests, rendering dialogs, and
   * processing user actions.
   */
  class FileDialogView : public IView 
  {
  public:
    FileDialogView() = default;
    virtual ~FileDialogView() = default;

    /**
     * @brief Draws the file dialog view and handles dialog rendering.
     */
    void draw() override;

    /**
     * @brief Destroys the file dialog view and performs cleanup.
     */
    void destroy() override;

    /**
     * @brief Checks if a file dialog request exists for the given dialog key.
     * 
     * @param dialogKey Unique key identifying the dialog request.
     * 
     * @return True if the request exists, false otherwise.
     */
    bool hasRequest(const String& dialogKey) const;

    /**
     * @brief Opens a new file dialog based on the provided request.
     * 
     * @param request The file dialog request to open.
     */
    void openFileDialog(const FileDialogRequest& request);

  private:
    UnorderedMap<String, FileDialogRequest> m_requests;
    Vector<String> m_requestForRemoval;

    void queueRequestForRemoval(const String& dialogKey);
    void removeRequests();
  };
}

#pragma once

#include "hc/editor/views/projectFileDialog/hcIProjectFileDialogRequest.h"

namespace hc::editor::projectFileDialog
{
  /**
   * @brief Standard implementation of the IProjectFileDialogRequest interface.
   */
  class ProjectFileDialogRequest : public IProjectFileDialogRequest
  {
  public:
    /**
     * @brief Constructs a ProjectFileDialogRequest with the specified parameters.
     *
     * @param title The title of the file dialog.
     * @param filters A vector of file extension filters for the dialog.
     * @param selectionCallback A callback function to be invoked when a file is selected.
     * @param cancelCallback A callback function to be invoked when the selection is
     * canceled.
     */
    ProjectFileDialogRequest(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&)>& selectionCallback,
      const std::function<void()>& cancelCallback
    );

    /**
     * @copydoc IProjectFileDialogRequest::~IProjectFileDialogRequest()
     */
    virtual ~ProjectFileDialogRequest() override;

    /**
     * @copydoc IProjectFileDialogRequest::getTitle()
     */
    virtual inline const String& getTitle() const override
    {
      return m_title;
    }

    /**
     * @copydoc IProjectFileDialogRequest::getFilters()
     */
    virtual inline const Vector<String>& getFilters() const override
    {
      return m_filters;
    }

    /**
     * @copydoc IProjectFileDialogRequest::onFileSelected(const Path& filePath)
     */
    virtual void onFileSelected(const Path& filePath) override;

    /**
     * @copydoc IProjectFileDialogRequest::onCancel()
     */
    virtual void onCancel() override;

    /**
     * @copydoc IProjectFileDialogRequest::destroy()
     */
    virtual void destroy() override;

  private:
    String m_title;
    Vector<String> m_filters;
    std::function<void(const Path&)> m_selectionCallback;
    std::function<void()> m_cancelCallback;
  };
}

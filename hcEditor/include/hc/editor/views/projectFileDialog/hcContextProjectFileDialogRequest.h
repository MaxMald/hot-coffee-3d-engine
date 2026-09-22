#pragma once

#include "hc/editor/views/projectFileDialog/hcIProjectFileDialogRequest.h"

namespace hc::editor::projectFileDialog
{
  /**
   * @brief Implementation of the IProjectFileDialogRequest interface with user-defined
   * context.
   */
  class ContextProjectFileDialogRequest : public IProjectFileDialogRequest
  {
  public:
    /**
     * @brief Constructs a ContextProjectFileDialogRequest with the specified parameters.
     *
     * @param title The title of the file dialog.
     * @param filters A vector of file extension filters for the dialog.
     * @param selectionCallback A callback function to be invoked when a file is selected,
     * with the user-defined context.
     * @param cancelCallback A callback function to be invoked when the selection is
     * canceled,
     * @param destroyContext A callback function to be invoked to destroy the user-defined
     * context.
     * @param context User-defined context to be passed to the callbacks.
     */
    ContextProjectFileDialogRequest(
      const String& title,
      const Vector<String>& filters,
      const std::function<void(const Path&, void*)>& selectionCallback,
      const std::function<void(void*)>& cancelCallback,
      const std::function<void(void*)>& destroyContext,
      void* context
    );

    /**
     * @copydoc IProjectFileDialogRequest::~IProjectFileDialogRequest()
     */
    virtual ~ContextProjectFileDialogRequest() override;

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
    std::function<void(const Path&, void*)> m_selectionCallback;
    std::function<void(void*)> m_cancelCallback;
    std::function<void(void*)> m_destroyContext;
    void* m_context;
  };
}

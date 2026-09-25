#include "hc/editor/views/projectFileDialog/hcContextProjectFileDialogRequest.h"

namespace hc::editor::projectFileDialog
{
  ContextProjectFileDialogRequest::ContextProjectFileDialogRequest(
    const String& title,
    const Vector<String>& filters,
    const std::function<void(const Path&, void*)>& selectionCallback,
    const std::function<void(void*)>& cancelCallback,
    const std::function<void(void*)>& destroyContext,
    void* context
  ) :
    m_title(title),
    m_filters(filters),
    m_selectionCallback(selectionCallback),
    m_cancelCallback(cancelCallback),
    m_destroyContext(destroyContext),
    m_context(context)
  {
  }

  ContextProjectFileDialogRequest::~ContextProjectFileDialogRequest()
  {
    destroy();
  }

  void ContextProjectFileDialogRequest::onFileSelected(const Path& filePath)
  {
    try
    {
      if (m_selectionCallback)
        m_selectionCallback(filePath, m_context);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "ContexProjectFileDialogRequest::onFileSelected: Error when calling \"onFileSelected\" callback: %s", e.what()
        )
      );
    }
  }

  void ContextProjectFileDialogRequest::onCancel()
  {
    try
    {
      if (m_cancelCallback)
        m_cancelCallback(m_context);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "ContexProjectFileDialogRequest::onCancel: Error when calling \"onCancel\" callback: %s", e.what()
        )
      );
    }
  }

  void ContextProjectFileDialogRequest::destroy()
  {
    m_title.clear();
    m_filters.clear();
    if (m_cancelCallback != nullptr)
      m_cancelCallback = nullptr;
    if (m_selectionCallback != nullptr)
      m_selectionCallback = nullptr;

    try
    {
      if (m_destroyContext != nullptr)
        m_destroyContext(m_context);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "ContexProjectFileDialogRequest::destroy: Error when calling \"destroyContext\" callback: %s", e.what()
        )
      );
    }

    m_destroyContext = nullptr;
    m_context = nullptr;
  }
}

#include "hc/editor/views/projectFileDialog/hcProjectFileDialogRequest.h"

namespace hc::editor::projectFileDialog
{
  ProjectFileDialogRequest::ProjectFileDialogRequest(
    const String& title,
    const Vector<String>& filters,
    const std::function<void(const Path&)>& selectionCallback,
    const std::function<void()>& cancelCallback
  ) :
    m_title(title),
    m_filters(filters),
    m_selectionCallback(selectionCallback),
    m_cancelCallback(cancelCallback)
  {
  }

  ProjectFileDialogRequest::~ProjectFileDialogRequest()
  {
    destroy();
  }

  void ProjectFileDialogRequest::onFileSelected(const Path& filePath)
  {
    try
    {
      if (m_selectionCallback)
        m_selectionCallback(filePath);
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "ProjectFileDialogRequest::onFileSelected: Error when calling \"onFileSelected\" callback: %s", e.what()
        )
      );
    }
  }

  void ProjectFileDialogRequest::onCancel()
  {
    try
    {
      if (m_cancelCallback)
        m_cancelCallback();
    }
    catch (const Exception& e)
    {
      LogService::Error(
        String::Format(
          "ProjectFileDialogRequest::onCancel: Error when calling \"onCancel\" callback: %s", e.what()
        )
      );
    }
  }

  void ProjectFileDialogRequest::destroy()
  {
    m_title.clear();
    m_filters.clear();
    if (m_cancelCallback != nullptr)
      m_cancelCallback = nullptr;
    if (m_selectionCallback != nullptr)
      m_selectionCallback = nullptr;
  }
}

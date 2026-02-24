#include "hc/editor/views/fileDialog/hcFileDialogView.h"
#include "ImGuiFileDialog.h"

namespace hc::editor
{
  void FileDialogView::draw()
  {
    for (auto it = m_requests.begin(); it != m_requests.end(); )
    {
      const String& key = it->first;
      FileDialogRequest& request = it->second;

      // Display the dialog for this key
      if (IGFD::FileDialog::Instance()->Display(key.c_str()))
      {
        if (IGFD::FileDialog::Instance()->IsOk())
        {
          String filePathNameStr = IGFD::FileDialog::Instance()->GetFilePathName();
          String currentPathStr = IGFD::FileDialog::Instance()->GetCurrentPath();

          Path filePathName(filePathNameStr.c_str());
          Path currentPath(currentPathStr.c_str());

          if (request.callback)
          {
            request.callback->onFileSelected(
              request.dialogKey,
              filePathName,
              currentPath
            );
          }
        }

        queueRequestForRemoval(key);
        IGFD::FileDialog::Instance()->Close();
      }
      else
      {
        ++it;
      }
    }

    removeRequests();
  }

  void FileDialogView::destroy()
  {
    m_requests.clear();
  }

  bool FileDialogView::hasRequest(const String& dialogKey) const
  {
    return m_requests.find(dialogKey) != m_requests.end();
  }

  void FileDialogView::openFileDialog(const FileDialogRequest& request)
  {
    m_requests[request.dialogKey] = request;

    IGFD::FileDialog::Instance()->OpenDialog(
      request.dialogKey.c_str(),
      request.dialogTitle.c_str(),
      request.fileExtensionFilter,
      request.igfdConfiguration
    );
  }

  void FileDialogView::queueRequestForRemoval(const String& dialogKey)
  {
    m_requestForRemoval.push_back(dialogKey);
  }

  void FileDialogView::removeRequests()
  {
    for (const String& dialogKey : m_requestForRemoval)
      m_requests.erase(dialogKey);
    m_requestForRemoval.clear();
  }
}

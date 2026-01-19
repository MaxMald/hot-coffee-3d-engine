#include "hc/editor/hcDirectoryReference.h"

#include <filesystem>
#include "hc/editor/hcFileReference.h"

namespace hc::editor
{
  DirectoryReference::DirectoryReference()
    : m_parent(nullptr)
  {
  }

  DirectoryReference::~DirectoryReference()
  {
  }

  bool DirectoryReference::initialize(
    const Path& directoryPath,
    DirectoryReference* parent
  )
  {
    clear();

    std::filesystem::path fsPath(directoryPath);
    if (!std::filesystem::exists(fsPath) || !std::filesystem::is_directory(fsPath))
    {
      return false;
    }

    m_fullPath = directoryPath;
    m_parent = parent;
    m_name = fsPath.filename().string();

    refresh();
    return true;
  }

  const String& DirectoryReference::getName() const
  {
    return m_name;
  }

  const Path& DirectoryReference::getFullPath() const
  {
    return m_fullPath;
  }

  DirectoryReference* DirectoryReference::getParent() const
  {
    return m_parent;
  }

  void DirectoryReference::refresh()
  {
    m_files.clear();
    m_subDirectories.clear();

    std::filesystem::path fsPath(m_fullPath);

    for (const auto& entry : std::filesystem::directory_iterator(fsPath))
    {
      if (entry.is_regular_file())
      {
        auto fileRef = MakeUnique<FileReference>();
        if (fileRef->initialize(entry.path().string(), this))
        {
          m_files.push_back(std::move(fileRef));
        }
      }
      else if (entry.is_directory())
      {
        auto dirRef = MakeUnique<DirectoryReference>();
        if (dirRef->initialize(entry.path().string(), this))
        {
          m_subDirectories.push_back(std::move(dirRef));
        }
      }
    }
  }

  const Vector<UniquePtr<FileReference>>& DirectoryReference::getFiles() const
  {
    return m_files;
  }

  const Vector<UniquePtr<DirectoryReference>>& DirectoryReference::getSubDirectories() const
  {
    return m_subDirectories;
  }

  void DirectoryReference::clear()
  {
    m_parent = nullptr;
    m_name.clear();
    m_fullPath.clear();
    m_files.clear();
    m_subDirectories.clear();
  }
}

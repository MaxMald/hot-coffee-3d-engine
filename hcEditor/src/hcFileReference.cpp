#include "hc/editor/hcFileReference.h"

#include <filesystem>
#include "hc/editor/hcDirectoryReference.h"

namespace hc::editor
{
  FileReference::FileReference()
    : m_parent(nullptr)
  {
  }

  FileReference::~FileReference()
  {
  }

  bool FileReference::initialize(const Path& filePath, DirectoryReference* parent)
  {
    clear();

    std::filesystem::path fsPath(filePath);
    if (!std::filesystem::exists(fsPath) || !std::filesystem::is_regular_file(fsPath))
    {
      return false;
    }

    m_fullPath = filePath;
    m_parent = parent;
    m_name = fsPath.stem().string();
    m_extension = fsPath.extension().string();
    m_nameWithExtension = fsPath.filename().string();
    return true;
  }

  const String& FileReference::getName() const
  {
    return m_name;
  }

  const String& FileReference::getNameWithExtension() const
  {
    return m_nameWithExtension;
  }

  const String& FileReference::getExtension() const
  {
    return m_extension;
  }

  const Path& FileReference::getFullPath() const
  {
    return m_fullPath;
  }

  DirectoryReference* FileReference::getParent() const
  {
    return m_parent;
  }

  void FileReference::clear()
  {
    m_parent = nullptr;
    m_name.clear();
    m_extension.clear();
    m_fullPath.clear();
    m_nameWithExtension.clear();
  }
}

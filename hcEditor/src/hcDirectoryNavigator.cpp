#include "hc/editor/hcDirectoryNavigator.h"

#include "hc/editor/hcFileReference.h"
#include "hc/editor/hcDirectoryReference.h"

namespace hc::editor
{
  DirectoryNavigator::DirectoryNavigator():
    m_rootDirectory(nullptr),
    m_currentDirectory(nullptr)
  {
  }

  DirectoryNavigator::~DirectoryNavigator()
  {
  }

  void DirectoryNavigator::initialize(const Path& rootDirectory)
  {
    clear();

    m_rootDirectory = MakeUnique<DirectoryReference>();
    if (m_rootDirectory->initialize(rootDirectory))
    {
      m_currentDirectory = m_rootDirectory.get();
    }
    else
    {
      m_rootDirectory.reset();
      m_currentDirectory = nullptr;
    }
  }

  DirectoryReference* DirectoryNavigator::getCurrentDirectory() const
  {
    return m_currentDirectory;
  }

  void DirectoryNavigator::navigateToParentDirectory()
  {
    if (m_currentDirectory != nullptr)
    {
      DirectoryReference* parent = m_currentDirectory->getParent();
      if (parent != nullptr)
      {
        m_currentDirectory = parent;
      }
    }
  }

  bool DirectoryNavigator::navigateToSubDirectory(const String& subDirectoryName)
  {
    if (m_currentDirectory != nullptr)
    {
      const Vector<UniquePtr<DirectoryReference>>& subDirectories = 
        m_currentDirectory->getSubDirectories();

      for (const UniquePtr<DirectoryReference>& subDirectory : subDirectories)
      {
        if (subDirectory->getName() == subDirectoryName)
        {
          m_currentDirectory = subDirectory.get();
          return true;
        }
      }
    }
    return false;
  }

  void DirectoryNavigator::clear()
  {
    m_rootDirectory.reset();
    m_currentDirectory = nullptr;
  }
}

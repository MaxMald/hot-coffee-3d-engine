#include "hc/editor/hcEditorLogger.h"

namespace hc::editor
{
  EditorLogger::EditorLogger(UInt32 capacity) :
    m_capacity(capacity),
    m_entries()
  {
  }

  EditorLogger::~EditorLogger()
  {
  }

  void EditorLogger::onMessageLogged(const String& message)
  {
    EditorLoggerEntry entry;
    entry.message = message;
    entry.type = editorLoggerEntryType::Message;
    addEntry(entry);
  }

  void EditorLogger::onWarningLogged(const String& message)
  {
    EditorLoggerEntry entry;
    entry.message = message;
    entry.type = editorLoggerEntryType::Warning;
    addEntry(entry);
  }

  void EditorLogger::onErrorLogged(const String& message)
  {
    EditorLoggerEntry entry;
    entry.message = message;
    entry.type = editorLoggerEntryType::Error;
    addEntry(entry);
  }

  UInt32 EditorLogger::getCapacity() const
  {
    return m_capacity;
  }

  SizeT EditorLogger::getEntryCount() const
  {
    return m_entries.size();
  }

  const Vector<EditorLoggerEntry>& EditorLogger::getEntries() const
  {
    return m_entries;
  }

  void EditorLogger::addEntry(const EditorLoggerEntry& entry)
  {
    if (m_entries.size() >= m_capacity)
      m_entries.erase(m_entries.begin());
    m_entries.push_back(entry);
  }

  void EditorLogger::clear()
  {
    m_entries.clear();
  }
}

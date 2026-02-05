#include "hc/editor/hcEditorLogger.h"

namespace hc::editor
{
  static constexpr UInt32 EDITOR_LOGGER_CAPACITY = 50;

  EditorLogger::EditorLogger() :
    m_capacity(EDITOR_LOGGER_CAPACITY),
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

  void EditorLogger::onPrepare()
  {
    LogService::Instance().subscribe(this);
  }

  void EditorLogger::onShutdown()
  {
    LogService::Instance().unsubscribe(this);
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

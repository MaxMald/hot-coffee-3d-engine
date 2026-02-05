#include "hc/editor/hcEditorLogHistory.h"

namespace hc::editor
{
  static constexpr UInt32 EDITOR_LOGGER_CAPACITY = 50;

  EditorLogHistory::EditorLogHistory() :
    m_capacity(EDITOR_LOGGER_CAPACITY),
    m_entries()
  {
  }

  EditorLogHistory::~EditorLogHistory()
  {
  }

  void EditorLogHistory::onMessageLogged(const String& message)
  {
    EditorLogHistoryEntry entry;
    entry.message = message;
    entry.type = editorLogHistoryEntryType::Message;
    addEntry(entry);
  }

  void EditorLogHistory::onWarningLogged(const String& message)
  {
    EditorLogHistoryEntry entry;
    entry.message = message;
    entry.type = editorLogHistoryEntryType::Warning;
    addEntry(entry);
  }

  void EditorLogHistory::onErrorLogged(const String& message)
  {
    EditorLogHistoryEntry entry;
    entry.message = message;
    entry.type = editorLogHistoryEntryType::Error;
    addEntry(entry);
  }

  UInt32 EditorLogHistory::getCapacity() const
  {
    return m_capacity;
  }

  SizeT EditorLogHistory::getEntryCount() const
  {
    return m_entries.size();
  }

  const Vector<EditorLogHistoryEntry>& EditorLogHistory::getEntries() const
  {
    return m_entries;
  }

  void EditorLogHistory::addEntry(const EditorLogHistoryEntry& entry)
  {
    if (m_entries.size() >= m_capacity)
      m_entries.erase(m_entries.begin());
    m_entries.push_back(entry);
  }

  void EditorLogHistory::clear()
  {
    m_entries.clear();
  }
}

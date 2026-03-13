#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/editorLogHistory/hcEditorLogHistoryEntry.h"

namespace hc::editor
{
  class EditorLogHistory : public ILogServiceListener
  {
  public:
    EditorLogHistory();
    virtual ~EditorLogHistory();

    void onMessageLogged(const String& message) override;
    void onWarningLogged(const String& message) override;
    void onErrorLogged(const String& message) override;

    UInt32 getCapacity() const;
    SizeT getEntryCount() const;
    const Vector<EditorLogHistoryEntry>& getEntries() const;
    void clear();

  private:
    UInt32 m_capacity;
    Vector<EditorLogHistoryEntry> m_entries;

    void addEntry(const EditorLogHistoryEntry& entry);
  };
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLoggerEntry.h"

namespace hc::editor
{
  class EditorLogger : public ILogServiceListener
  {
  public:
    EditorLogger(UInt32 m_capacity);
    ~EditorLogger() override;

    void onMessageLogged(const String& message) override;
    void onWarningLogged(const String& message) override;
    void onErrorLogged(const String& message) override;

    UInt32 getCapacity() const;
    SizeT getEntryCount() const;
    const Vector<EditorLoggerEntry>& getEntries() const;
    void clear();

  private:
    UInt32 m_capacity;
    Vector<EditorLoggerEntry> m_entries;

    void addEntry(const EditorLoggerEntry& entry);
  };
}

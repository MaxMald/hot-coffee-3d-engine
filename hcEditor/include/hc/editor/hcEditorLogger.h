#pragma once

#include "hc/editor/hcEditorPrerequisites.h"
#include "hc/editor/hcEditorLoggerEntry.h"

namespace hc::editor
{
  class EditorLogger :
    public ILogServiceListener,
    public AModule<EditorLogger>
  {
  public:
    EditorLogger();
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

    void onPrepare() override;
    void onShutdown() override;
    void addEntry(const EditorLoggerEntry& entry);
  };
}

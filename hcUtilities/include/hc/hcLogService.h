#pragma once

#include "hc/hcUtilitiesPrerequisites.h"
#include "hc/hcString.h"
#include "hc/hcNonCopyable.h"

namespace hc
{
  class ILogServiceListener;

  class HC_UTILITY_EXPORT LogService : public NonCopyable
  {
  public:
    static LogService& Instance();
    static void Prepare();
    static void Shutdown();

    static void Message(const String& message);
    static void Warning(const String& message);
    static void Error(const String& message);

    void subscribe(ILogServiceListener* listener);
    void unsubscribe(ILogServiceListener* listener);

  private:
    static LogService* _Instance;

    Vector<ILogServiceListener*> m_listeners;

    LogService();
    ~LogService();
  };
}

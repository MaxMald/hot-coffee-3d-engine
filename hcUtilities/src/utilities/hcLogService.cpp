#include "hc/utilities/hcLogService.h"
#include "hc/utilities/hcILogServiceListener.h"

namespace hc
{
  void LogService::Message(const String& message)
  {
    if (s_instance)
    {
      for (auto* listener : s_instance->m_listeners)
      {
        if (listener)
          listener->onMessageLogged(message);
      }
    }
  }

  void LogService::Warning(const String& message)
  {
    if (s_instance)
    {
      for (auto* listener : s_instance->m_listeners)
      {
        if (listener)
          listener->onWarningLogged(message);
      }
    }
  }

  void LogService::Error(const String& message)
  {
    if (s_instance)
    {
      for (auto* listener : s_instance->m_listeners)
      {
        if (listener)
          listener->onErrorLogged(message);
      }
    }
  }

  LogService::LogService() = default;

  LogService::~LogService() = default;

  void LogService::subscribe(ILogServiceListener* listener)
  {
    if (listener && std::find(
      m_listeners.begin(),
      m_listeners.end(),
      listener) == m_listeners.end()
      )
    {
      m_listeners.push_back(listener);
    }
  }

  void LogService::unsubscribe(ILogServiceListener* listener)
  {
    auto it = std::remove(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end())
    {
      m_listeners.erase(it, m_listeners.end());
    }
  }

  void LogService::onPrepare()
  {
    // intentional left blank
  }

  void LogService::onShutdown()
  {
    // intentional left blank
  }
}

#include "hc/hcLogService.h"
#include "hc/hcILogServiceListener.h"

namespace hc
{
  LogService* LogService::_Instance = nullptr;

  LogService& LogService::Instance()
  {
    if (!_Instance)
      throw RuntimeErrorException(
        "LogService is not prepared. Call LogService::Prepare() before using it."
      );

    return *_Instance;
  }

  void LogService::Prepare()
  {
    if (!_Instance)
      _Instance = new LogService();
  }

  void LogService::Shutdown()
  {
    if (_Instance)
    {
      delete _Instance;
      _Instance = nullptr;
    }
  }

  void LogService::Message(const String& message)
  {
    if (_Instance)
    {
      for (auto* listener : _Instance->m_listeners)
      {
        if (listener)
          listener->onMessageLogged(message);
      }
    }
  }

  void LogService::Warning(const String& message)
  {
    if (_Instance)
    {
      for (auto* listener : _Instance->m_listeners)
      {
        if (listener)
          listener->onWarningLogged(message);
      }
    }
  }

  void LogService::Error(const String& message)
  {
    if (_Instance)
    {
      for (auto* listener : _Instance->m_listeners)
      {
        if (listener)
          listener->onErrorLogged(message);
      }
    }
  }

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

  LogService::LogService() = default;

  LogService::~LogService() = default;
}

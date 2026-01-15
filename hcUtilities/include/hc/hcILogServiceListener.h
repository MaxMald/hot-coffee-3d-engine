#pragma once

#include "hc/hcString.h"

namespace hc
{
  class ILogServiceListener
  {
  public:
    virtual ~ILogServiceListener() = default;

    virtual void onMessageLogged(const String& message) = 0;
    virtual void onWarningLogged(const String& message) = 0;
    virtual void onErrorLogged(const String& message) = 0;
  };
}

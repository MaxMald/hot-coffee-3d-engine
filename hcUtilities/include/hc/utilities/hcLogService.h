#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcString.h"
#include "hc/utilities/hcAModule.h"

namespace hc
{
  class ILogServiceListener;

  /**
   * @brief Singleton service for logging messages, warnings, and errors.
   */
  class HC_UTILITY_EXPORT LogService : public AModule<LogService>
  {
  public:
    /**
     * @brief Log a general message.
     * 
     * @param message The message to log.
     */
    static void Message(const String& message);

    /**
     * @brief Log a warning message.
     * 
     * @param message The warning message to log.
     */
    static void Warning(const String& message);

    /**
     * @brief Log an error message.
     * 
     * @param message The error message to log.
     */
    static void Error(const String& message);

    LogService();
    ~LogService() override;

    /**
     * @brief Subscribe a listener to receive log events.
     * 
     * @param listener Pointer to the listener to subscribe.
     */
    void subscribe(ILogServiceListener* listener);

    /**
     * @brief Unsubscribe a listener from receiving log events.
     * 
     * @param listener Pointer to the listener to unsubscribe.
     */
    void unsubscribe(ILogServiceListener* listener);

  private:
    Vector<ILogServiceListener*> m_listeners;

    void onPrepare() override;
    void onShutdown() override;
  };
}

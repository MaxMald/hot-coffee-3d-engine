#pragma once

#include "hc/utilities/hcUtilitiesPrerequisites.h"
#include "hc/utilities/hcNonCopyable.h"

namespace hc
{
  /**
   * @brief Abstract base class for singleton modules.
   *
   * Inherit from this class to implement a singleton module with controlled
   * initialization and shutdown. The derived class must implement the
   * onPrepare() and onShutdown() methods for custom setup and cleanup logic.
   *
   * @tparam T The derived module type.
   */
  template<typename T>
  class AModule : public NonCopyable
  {
  public:
    /**
     * @brief Get the singleton instance of the module.
     *
     * @return Reference to the singleton instance.
     * @throws hc::RuntimeErrorException if the module has not been prepared.
     */
    static T& Instance();

    /**
      * @brief Check if the module singleton instance has been created.
      *
      * @return true if the instance exists, false otherwise.
      */
    static bool HasInstance();

    /**
     * @brief Prepare and initialize the module singleton.
     *
     * Allocates the singleton instance and calls onPrepare().
     * Safe to call multiple times; only the first call has effect.
     */
    static void Prepare();

    /**
     * @brief Shutdown and destroy the module singleton.
     *
     * Calls onShutdown() and deletes the singleton instance.
     * Safe to call multiple times; only the first call after Prepare() has effect.
     */
    static void Shutdown();

  protected:
    /**
     * @brief Pointer to the singleton instance.
     */
    static T* s_instance;

    AModule() = default;
    virtual ~AModule() = default;

    /**
     * @brief Custom initialization logic for the module.
     *
     * Must be implemented by the derived class.
     */
    virtual void onPrepare() = 0;

    /**
     * @brief Custom cleanup logic for the module.
     *
     * Must be implemented by the derived class.
     */
    virtual void onShutdown() = 0;
  };

  template<typename T>
  T* AModule<T>::s_instance = nullptr;

  template<typename T>
  T& AModule<T>::Instance()
  {
    if (s_instance == nullptr)
    {
      throw hc::RuntimeErrorException(
        "Module not prepared. Call Prepare() before accessing the instance."
      );
    }
    return *s_instance;
  }

  template<typename T>
  bool AModule<T>::HasInstance()
  {
    return s_instance != nullptr;
  }

  template<typename T>
  void AModule<T>::Prepare()
  {
    if (s_instance == nullptr)
    {
      s_instance = new T();

      AModule* module = static_cast<AModule*>(s_instance);
      module->onPrepare();
    }
  }

  template<typename T>
  void AModule<T>::Shutdown()
  {
    if (s_instance != nullptr)
    {
      AModule* module = static_cast<AModule*>(s_instance);
      module->onShutdown();

      delete s_instance;
      s_instance = nullptr;
    }
  }
}

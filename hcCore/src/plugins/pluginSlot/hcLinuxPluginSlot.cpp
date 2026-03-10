#include "hc/plugins/pluginSlot/hcLinuxPluginSlot.h"

#if HC_PLATFORM == HC_PLATFORM_LINUX

#include <dlfcn.h>

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  typedef IPlugin* (*fnCreatePlugin)(void);
  typedef void (*fnDestroyPlugin)(IPlugin*);

  LinuxPluginSlot::LinuxPluginSlot()
    : m_pluginHandler(nullptr),
      m_pluginPtr(nullptr),
      m_isConnected(false)
  {
  }

  LinuxPluginSlot::~LinuxPluginSlot()
  {
    if (m_isConnected)
    {
      LogService::Warning(
        "LinuxPluginSlot destroyed while still connected. Closing plugin."
      );

      close();
    }
  }

  bool LinuxPluginSlot::connect(
    const String& key,
    const String& libraryName,
    const String& constructorFunctionName,
    const String& destructorFunctionName
  )
  {
    if (m_isConnected)
    {
      LogService::Warning(
        "Attempted to connect LinuxPluginSlot, but it is already connected."
      );

      return false;      
    }

    LogService::Message(
      "Connecting LinuxPluginSlot: loading library '" + libraryName + "'."
    );

    void* pluginHandle = dlopen(libraryName.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (pluginHandle == nullptr)
    {
      const char* dlError = dlerror();
      LogService::Error(
        "Failed to load plugin library: " + libraryName +
        (dlError ? String(". dlerror: ") + dlError : "")
      );

      return false;
    }

    dlerror();
    void* constructorSymbol = dlsym(
      pluginHandle, 
      constructorFunctionName.c_str()
    );

    const char* constructorError = dlerror();
    if (constructorError != nullptr)
    {
      LogService::Error(
        "Constructor function '" + constructorFunctionName + "' not found in library: " + libraryName +
        ". dlerror: " + constructorError
      );

      dlclose(pluginHandle);
      return false;
    }

    fnCreatePlugin constructorFunction = reinterpret_cast<fnCreatePlugin>(constructorSymbol);
    IPlugin* pluginPtr = constructorFunction();

    if (pluginPtr == nullptr)
    {
      LogService::Error(
        "Constructor function '" + constructorFunctionName + "' returned nullptr for library: " + libraryName
      );

      dlclose(pluginHandle);
      return false;
    }

    LogService::Message("Plugin constructed and connected: " + key);

    pluginPtr->onConnect();

    m_key = key;
    m_libraryName = libraryName;
    m_constructorFunctionName = constructorFunctionName;
    m_destructorFunctionName = destructorFunctionName;
    m_pluginHandler = pluginHandle;
    m_pluginPtr = pluginPtr;

    m_isConnected = true;
    return true;
  }

  const String& LinuxPluginSlot::getKey() const
  {
    return m_key;
  }

  const String& LinuxPluginSlot::getLibraryName() const
  {
    return m_libraryName;
  }

  const String& LinuxPluginSlot::getConstructorFunctionName() const
  {
    return m_constructorFunctionName;
  }

  const String& LinuxPluginSlot::getDestructorFunctionName() const
  {
    return m_destructorFunctionName;
  }

  IPlugin& LinuxPluginSlot::getPlugin()
  {
    if (m_pluginPtr == nullptr)
      throw RuntimeErrorException("Plugin slot is not connected.");

    return *m_pluginPtr;
  }

  void LinuxPluginSlot::close()
  {
    if (!m_isConnected)
    {
      LogService::Warning(
        "Attempted to close LinuxPluginSlot, but it is not connected."
      );

      return;
    }

    LogService::Message("Closing LinuxPluginSlot for key: " + m_key);

    m_pluginPtr->onClose();

    dlerror();
    void* destructorSymbol = dlsym(m_pluginHandler, m_destructorFunctionName.c_str());
    const char* destructorError = dlerror();
    if (destructorError == nullptr)
    {
      fnDestroyPlugin destructionFunction = reinterpret_cast<fnDestroyPlugin>(destructorSymbol);
      destructionFunction(m_pluginPtr);

      LogService::Message("Plugin destruction function called for key: " + m_key);
    }
    else
    {
      LogService::Warning(
        "Destructor function '" + m_destructorFunctionName + "' not found in library: " + m_libraryName +
        ". dlerror: " + destructorError
      );
    }

    m_pluginPtr = nullptr;
    dlclose(m_pluginHandler);
    m_pluginHandler = nullptr;
    m_isConnected = false;

    LogService::Message("LinuxPluginSlot closed for key: " + m_key);
  }
}

#endif // HC_PLATFORM == HC_PLATFORM_LINUX

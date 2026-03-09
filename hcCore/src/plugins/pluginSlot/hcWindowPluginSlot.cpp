#include "hc/plugins/pluginSlot/hcWindowPluginSlot.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32

#include "hc/plugins/hcIPlugin.h"

namespace hc
{
  typedef IPlugin* (*fnCreatePlugin)(void);
  typedef void (*fnDestroyPlugin)(IPlugin*);

  WindowsPluginSlot::WindowsPluginSlot() :
    m_pluginHandler(nullptr),
    m_pluginPtr(nullptr),
    m_isConnected(false)
  {
  }

  WindowsPluginSlot::~WindowsPluginSlot()
  {
    if (m_isConnected)
    {
      LogService::Warning(
        "WindowsPluginSlot destroyed while still connected. Closing plugin."
      );

      close();
    }
  }

  bool WindowsPluginSlot::connect(
    const String& key,
    const String& libraryName,
    const String& constructorFunctionName,
    const String& destructorFunctionName
  )
  {
    if (m_isConnected)
    {
      LogService::Warning(
        "Attempted to connect WindowsPluginSlot, but it is already connected."
      );

      return false;
    }

    LogService::Message(
      "Connecting WindowsPluginSlot: loading library '" + libraryName + "'."
    );

    std::wstring wLibraryName(libraryName.begin(), libraryName.end());
    HINSTANCE pluginHandle = LoadLibraryExW(
      wLibraryName.c_str(),
      0,
      LOAD_WITH_ALTERED_SEARCH_PATH
    );

    if (pluginHandle == NULL)
    {
      LogService::Error(
        "Failed to load plugin library: " + libraryName
      );

      return false;
    }

    FARPROC proc = GetProcAddress(
      pluginHandle, 
      constructorFunctionName.c_str()
    );

    if (proc == NULL)
    {
      LogService::Error(
        "Constructor function '" + constructorFunctionName + "' not found in library: " + libraryName
      );

      FreeLibrary(pluginHandle);
      return false;
    }

    fnCreatePlugin constructorFunction = reinterpret_cast<fnCreatePlugin>(proc);
    if (constructorFunction == nullptr)
    {
      LogService::Error(
        "Failed to cast constructor function '" + constructorFunctionName + "' in library: " + libraryName
      );

      FreeLibrary(pluginHandle);
      return false;
    }

    IPlugin* pluginPtr = constructorFunction();      
    if (pluginPtr == nullptr)
    {
      LogService::Error(
        "Constructor function '" + constructorFunctionName + "' returned nullptr for library: " + libraryName
      );

      FreeLibrary(pluginHandle);
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

  const String& WindowsPluginSlot::getKey() const
  {
    return m_key;
  }

  const String& WindowsPluginSlot::getLibraryName() const
  {
    return m_libraryName;
  }

  const String& WindowsPluginSlot::getConstructorFunctionName() const
  {
    return m_constructorFunctionName;
  }

  const String& WindowsPluginSlot::getDestructorFunctionName() const
  {
    return m_destructorFunctionName;
  }

  IPlugin& WindowsPluginSlot::getPlugin()
  {
    if (m_pluginPtr == nullptr)
      throw RuntimeErrorException("Plugin slot is not connected.");

    return *m_pluginPtr;
  }

  void WindowsPluginSlot::close()
  {
    if (!m_isConnected)
    {
      LogService::Warning(
        "Attempted to close WindowsPluginSlot, but it is not connected."
      );

      return;
    }

    LogService::Message("Closing WindowsPluginSlot for key: " + m_key);

    FARPROC proc = GetProcAddress(
      m_pluginHandler,
      m_destructorFunctionName.c_str()
    );

    if (proc == NULL)
    {
      LogService::Error(
        "Destructor function '" + m_destructorFunctionName + "' not found in library: " + m_libraryName
      );
    }
    else
    {
      fnDestroyPlugin destructionFunction = reinterpret_cast<fnDestroyPlugin>(proc);
      m_pluginPtr->onClose();
      destructionFunction(m_pluginPtr);
      LogService::Message("Plugin destruction function called for key: " + m_key);
    }
    
    m_pluginPtr = nullptr;

    FreeLibrary(m_pluginHandler);
    m_isConnected = false;

    LogService::Message("WindowsPluginSlot closed for key: " + m_key);
  }
}

#endif // HC_PLATFORM == HC_PLATFORM_WIN32

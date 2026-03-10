#pragma once

#include "hc/hcCorePrerequisites.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32

#include <Windows.h>
#include <libloaderapi.h>

#include "hc/plugins/pluginSlot/hcIPluginSlot.h"

namespace hc
{
  class WindowsPluginSlot : public IPluginSlot
  {
  public:
    WindowsPluginSlot();
    ~WindowsPluginSlot() override;

    virtual bool connect(
        const String& key,
        const String& libraryName,
        const String& constructorFunctionName,
        const String& destructorFunctionName
      ) override;

    virtual const String& getKey() const override;
    virtual const String& getLibraryName() const override;
    virtual const String& getConstructorFunctionName() const override;
    virtual const String& getDestructorFunctionName() const override;
    virtual IPlugin& getPlugin() override;
    virtual void close() override;

  private:
    /**
    * The name of the constructor function.
    */
    String m_constructorFunctionName;

    /**
    * The name of the destructor function.
    */
    String m_destructorFunctionName; 

    /**
    * The Plug-in's key.
    */
    String m_key;

    /**
    * The library name of the plug-in.
    */
    String m_libraryName;

    /**
    * The instance of the plug-in.
    */
    HINSTANCE m_pluginHandler;

    /**
    * Pointer of the Plug-in interface.
    */
    IPlugin* m_pluginPtr;

    /**
    * Indicates if the plug-in slot is connected.
    */
    bool m_isConnected;

    /**
     * Checks if the specified library contains a destructor function with the
     * given name. Returns true if the function is found, otherwise returns false.
     *
     * @param destructorFunctionName The name of the destructor function to check for.
     * @param pluginHandle The handle to the loaded library.
     *
     * @return true if the destructor function is found in the library, otherwise false.
     */
    bool checkLibraryHasDestructorFunction(
      const String& destructorFunctionName,
      HINSTANCE pluginHandle
    ) const;
  };
}

#endif // HC_PLATFORM == HC_PLATFORM_WIN32

#pragma once

#include "hc/hcCorePrerequisites.h"

#if HC_PLATFORM == HC_PLATFORM_LINUX

#include "hc/hcIPluginSlot.h"

namespace hc
{
  class LinuxPluginSlot : public IPluginSlot
  {
  public:
    LinuxPluginSlot();
    ~LinuxPluginSlot() override;

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
    virtual SharedPtr<IPlugin> getPluginPtr() override;
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
    void* m_pluginHandler;

    /**
    * Pointer of the Plug-in interface.
    */
    SharedPtr<IPlugin> m_pluginPtr;

    /**
    * Indicates if the plug-in slot is connected.
    */
    bool m_isConnected;
  };
}

#endif // HC_PLATFORM == HC_PLATFORM_LINUX
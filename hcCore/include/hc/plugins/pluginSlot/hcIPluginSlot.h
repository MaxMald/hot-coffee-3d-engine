#pragma once

#include "hc/hcCorePrerequisites.h"

namespace hc
{
  class IPlugin;

  /**
  * A plug-in slot provides a connection to a single IPlugin, that could come
  * from an external library.
  *
  * The plug-in slot manage the proper connection and disconnection of a single
  * IPlugin, also provides hints if something go wrong with the plug-in
  * connection.
  */
  class HC_CORE_EXPORT IPluginSlot
  {
  public:
    virtual ~IPluginSlot() = default;

    /**
    * Attempts to connect to the specified plug-in.
    *
    * @param _key The identifier of this plug-in.
    * @param _libraryName The name of the library of the plug-in.
    * @param _constructorFunctionName The name of the function that builds the
    * plug-in.
    * @param _destructorFunctionName The name of the function that destroys the
    * plug-in.
    *
    * @return true if the connection was successful.
    */
    virtual bool connect(
      const String& _key,
      const String& _libraryName,
      const String& _constructorFunctionName,
      const String& _destructorFunctionName
    ) = 0;

    /**
    * Get the identifier of this Plug-in slot.
    *
    * @return The identifier of this Plug-in slot.
    */
    virtual const String& getKey() const = 0;

    /**
    * Get the library name of this Plug-in slot.
    * 
    * @return The library name of this Plug-in slot.
    */
    virtual const String& getLibraryName() const = 0;

    /**
     * Get the constructor function name of this Plug-in slot.
     * 
     * @return The constructor function name of this Plug-in slot.
     */
    virtual const String& getConstructorFunctionName() const = 0;

    /**
     * Get the destructor function name of this Plug-in slot.
     * 
     * @return The destructor function name of this Plug-in slot.
     */
    virtual const String& getDestructorFunctionName() const = 0;

    /**
    * Get the pointer of the wrapped plug-in.
    *
    * @return The pointer of the wrapped plug-in.
    */
    virtual SharedPtr<IPlugin> getPluginPtr() = 0;

    /**
    * Safely disconnect this plug-in.
    */
    virtual void close() = 0;
  };
}

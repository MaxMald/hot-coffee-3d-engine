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
  class IPluginSlot
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
    virtual String getKey() = 0;

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

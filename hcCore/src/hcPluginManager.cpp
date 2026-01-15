#include "hc/hcPluginManager.h"
#include "hc/hcIPluginSlotFactory.h"
#include "hc/hcIPluginSlot.h"
#include "hc/hcIPlugin.h"
#include "hc/hcDependencyContainer.h"

#if HC_PLATFORM == HC_PLATFORM_WIN32
#include "hc/hcWindowsPluginSlotFactory.h"
#endif

namespace hc
{
  PluginManager::PluginManager() :
    m_pluginSlotFactory(nullptr)
  {
  }

  PluginManager::~PluginManager()
  {
  }

  void PluginManager::init()
  {

#if HC_PLATFORM == HC_PLATFORM_WIN32
    m_pluginSlotFactory = MakeShared<WindowsPluginSlotFactory>();
#endif

  }

  bool PluginManager::connectPlugin(
    const String& _key,
    const String& _libraryName,
    const String& _constructorFunctionName,
    const String& _destructorFunctionName
  )
  {
    if (!m_pluginSlotFactory)
    {
      LogService::Error(
        "Cannot connect to plugin. PluginSlotFactory is nullptr."
      );

      return false;
    }

    if (this->hasPlugin(_key))
    {
      LogService::Error(
        String::Format(
          "Cannot connect to plugin. Plugin with key '%s' already exists.",
          _key.c_str()
        )
      );

      return false;
    }

    SharedPtr<IPluginSlot> pPluginSlot = m_pluginSlotFactory->createPluginSlot();
    bool connected = pPluginSlot->connect
    (
      _key,
      _libraryName,
      _constructorFunctionName,
      _destructorFunctionName
    );

    if (!connected)
      return false;

    m_pluginSlots.insert({ _key, pPluginSlot });
    return connected;
  }

  bool PluginManager::closePlugin(const String& _key)
  {
    auto it = m_pluginSlots.find(_key);
    if (it == m_pluginSlots.end())
      return false;

    SharedPtr<IPluginSlot> pPluginSlot = it->second;
    if (pPluginSlot)
      pPluginSlot->close();

    m_pluginSlots.erase(it);
    return true;
  }

  void PluginManager::closeAll()
  {
    for (auto& [key, pPluginSlot] : m_pluginSlots)
    {
      if (pPluginSlot)
        pPluginSlot->close();
    }

    m_pluginSlots.clear();
  }

  bool PluginManager::hasPlugin(const String& _key)
  {
    return m_pluginSlots.find(_key) != m_pluginSlots.end();
  }

  SharedPtr<IPlugin> PluginManager::getPlugin(const String& _key)
  {
    auto it = m_pluginSlots.find(_key);
    if (it == m_pluginSlots.end())
      return nullptr;

    SharedPtr<IPluginSlot> pPluginSlot = it->second;
    if (!pPluginSlot)
      return nullptr;

    return pPluginSlot->getPluginPtr();
  }

  void PluginManager::addDependenciesFromPlugins(DependencyContainer& container)
  {
    for (auto& [key, pPluginSlot] : m_pluginSlots)
    {
      if (pPluginSlot)
      {
        SharedPtr<IPlugin> pPlugin = pPluginSlot->getPluginPtr();
        if (pPlugin)
        {
          pPlugin->addDependencies(container);
        }
      }
    }
  }
}

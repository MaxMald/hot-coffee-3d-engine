#include "hc/graphics/environment/hcOpenGlEnvironmentManager.h"

#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc::graphics
{
  OpenGlEnvironmentManager::OpenGlEnvironmentManager() :
    m_environmentCubeMap(nullptr),
    m_diffuseIrradianceCubeMap(nullptr),
    m_prefilteredEnvironmentCubeMap(nullptr),
    m_blackImage(1, 1, textureFormatType::RGB8, colorSpaceType::Linear, Color(0.0f)),
    m_blackCubeMap(),
    m_isInitialized(false)
  {
  }

  OpenGlEnvironmentManager::~OpenGlEnvironmentManager()
  {
    destroy();
  }

  void OpenGlEnvironmentManager::initialize()
  {
    destroy();

    try
    {
      m_blackCubeMap.initialize(
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage,
        m_blackImage
      );
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        "Failed to initialize black cube map for environment textures: " + String(e.what())
      );
    }

    m_isInitialized = true;
  }

  void OpenGlEnvironmentManager::setEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for environment textures.");
    m_environmentCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::setDiffuseIrradianceCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for diffuse irradiance textures.");
    m_diffuseIrradianceCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::setPrefilteredEnvironmentCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for prefiltered environment textures.");
    m_prefilteredEnvironmentCubeMap = cubeMap;
  }

  void OpenGlEnvironmentManager::clearEnvironmentCubeMap()
  {
    m_environmentCubeMap.reset();
  }

  void OpenGlEnvironmentManager::clearDiffuseIrradianceCubeMap()
  {
    m_diffuseIrradianceCubeMap.reset();
  }

  void OpenGlEnvironmentManager::clearPrefilteredEnvironmentCubeMap()
  {
    m_prefilteredEnvironmentCubeMap.reset();
  }

  void OpenGlEnvironmentManager::bindEnvironmentResources() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("Environment manager is not initialized.");

    if (m_environmentCubeMap != nullptr && m_environmentCubeMap->isValid())
      m_environmentCubeMap->bind(SLOT_ENVIRONMENT_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_CUBE_MAP);

    if (m_diffuseIrradianceCubeMap != nullptr && m_diffuseIrradianceCubeMap->isValid())
      m_diffuseIrradianceCubeMap->bind(SLOT_ENVIRONMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);

    if (m_prefilteredEnvironmentCubeMap != nullptr && m_prefilteredEnvironmentCubeMap->isValid())
      m_prefilteredEnvironmentCubeMap->bind(SLOT_ENVIRONMENT_PREFILTERED_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIRONMENT_PREFILTERED_CUBE_MAP);
  }

  void OpenGlEnvironmentManager::destroy()
  {
    m_environmentCubeMap.reset();
    m_diffuseIrradianceCubeMap.reset();
    m_prefilteredEnvironmentCubeMap.reset();
    m_blackCubeMap.destroy();
    m_isInitialized = false;
  }
}

#include "hc/graphics/enviroment/hcOpenGlEnviromentManager.h"

#include "hc/graphics/resource/texture/hcOpenGlTexture.h"

namespace hc::graphics
{
  OpenGlEnviromentManager::OpenGlEnviromentManager() :
    m_enviromentCubeMap(nullptr),
    m_diffuseIrradianceCubeMap(nullptr),
    m_blackImage(1, 1, textureFormatType::RGB8, colorSpaceType::Linear, Color(0.0f)),
    m_blackCubeMap(),
    m_isInitialized(false)
  {
  }

  OpenGlEnviromentManager::~OpenGlEnviromentManager()
  {
    destroy();
  }

  void OpenGlEnviromentManager::initialize()
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

  void OpenGlEnviromentManager::setEnviromentCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for environment textures.");
    m_enviromentCubeMap = cubeMap;
  }

  void OpenGlEnviromentManager::setDiffuseIrradianceCubeMap(SharedPtr<ICubeMap> cubeMap)
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw InvalidArgumentException("Invalid cube map provided for diffuse irradiance textures.");
    m_diffuseIrradianceCubeMap = cubeMap;
  }

  void OpenGlEnviromentManager::clearEnviromentCubeMap()
  {
    m_enviromentCubeMap.reset();
  }

  void OpenGlEnviromentManager::clearDiffuseIrradianceCubeMap()
  {
    m_diffuseIrradianceCubeMap.reset();
  }

  void OpenGlEnviromentManager::bindEnviromentResources() const
  {
    if (!m_isInitialized)
      throw RuntimeErrorException("Environment manager is not initialized.");

    if (m_enviromentCubeMap != nullptr && m_enviromentCubeMap->isValid())
      m_enviromentCubeMap->bind(SLOT_ENVIROMENT_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIROMENT_CUBE_MAP);

    if (m_diffuseIrradianceCubeMap != nullptr && m_diffuseIrradianceCubeMap->isValid())
      m_diffuseIrradianceCubeMap->bind(SLOT_ENVIROMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);
    else
      m_blackCubeMap.bind(SLOT_ENVIROMENT_DIFFUSE_IRRADIANCE_CUBE_MAP);
  }

  void OpenGlEnviromentManager::destroy()
  {
    m_enviromentCubeMap.reset();
    m_diffuseIrradianceCubeMap.reset();
    m_blackCubeMap.destroy();
    m_isInitialized = false;
  }
}

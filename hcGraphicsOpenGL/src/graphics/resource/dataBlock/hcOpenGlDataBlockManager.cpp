#include "hc/graphics/resource/dataBlock/hcOpenGlDataBlockManager.h"

#include <hc/graphics/resource/dataBlock/hcDataBlockStructures.h>

namespace hc
{
  void OpenGlDataBlockManager::initialize()
  {
    if (m_isInitialized)
      throw RuntimeErrorException("OpenGlDataBlockManager is already initialized.");

    dataBlockStructure::Camera cameraInitData;
    dataBlockStructure::Lights lightsInitData;
    dataBlockStructure::LightShadows lightShadowsInitData;
    dataBlockStructure::ObjectData objectInitData;
    dataBlockStructure::LightViewProjection lightViewProjectionInitData;
    dataBlockStructure::MaterialUnlit materialUnlitInitData;
    dataBlockStructure::MaterialBlinnPhong materialBlinnPhongInitData;
    dataBlockStructure::MaterialHair materialHairInitData;

    try
    {
      UniquePtr<OpenGlDataBlock> camera = MakeUnique<OpenGlDataBlock>();
      camera->initialize(&cameraInitData, sizeof(cameraInitData));

      UniquePtr<OpenGlDataBlock> lights = MakeUnique<OpenGlDataBlock>();
      lights->initialize(&lightsInitData, sizeof(lightsInitData));

      UniquePtr<OpenGlDataBlock> lightShadows = MakeUnique<OpenGlDataBlock>();
      lightShadows->initialize(&lightShadowsInitData, sizeof(lightShadowsInitData));

      UniquePtr<OpenGlDataBlock> object = MakeUnique<OpenGlDataBlock>();
      object->initialize(&objectInitData, sizeof(objectInitData));

      UniquePtr<OpenGlDataBlock> lightViewProjection = MakeUnique<OpenGlDataBlock>();
      lightViewProjection->initialize(&lightViewProjectionInitData, sizeof(lightViewProjectionInitData));

      UniquePtr<OpenGlDataBlock> materialUnlit = MakeUnique<OpenGlDataBlock>();
      materialUnlit->initialize(&materialUnlitInitData, sizeof(materialUnlitInitData));

      UniquePtr<OpenGlDataBlock> materialBlinnPhong = MakeUnique<OpenGlDataBlock>();
      materialBlinnPhong->initialize(&materialBlinnPhongInitData, sizeof(materialBlinnPhongInitData));

      UniquePtr<OpenGlDataBlock> materialHair = MakeUnique<OpenGlDataBlock>();
      materialHair->initialize(&materialHairInitData, sizeof(materialHairInitData));

      if (!camera->isValid() || !lights->isValid() || !lightShadows->isValid()
        || !object->isValid() || !lightViewProjection->isValid()
        || !materialUnlit->isValid() || !materialBlinnPhong->isValid()
        || !materialHair->isValid())
      {
        throw RuntimeErrorException("Failed to initialize one or more OpenGL data blocks.");
      }

      m_dataBlocks[dataBlockType::Camera] = std::move(camera);
      m_dataBlocks[dataBlockType::Lights] = std::move(lights);
      m_dataBlocks[dataBlockType::LightShadows] = std::move(lightShadows);
      m_dataBlocks[dataBlockType::Object] = std::move(object);
      m_dataBlocks[dataBlockType::LightViewProjection] = std::move(lightViewProjection);
      m_dataBlocks[dataBlockType::MaterialUnlit] = std::move(materialUnlit);
      m_dataBlocks[dataBlockType::MaterialBlinnPhong] = std::move(materialBlinnPhong);
      m_dataBlocks[dataBlockType::MaterialHair] = std::move(materialHair);
    }
    catch (const Exception& e)
    {
      destroy();
      throw RuntimeErrorException(
        "Failed to initialize OpenGL data blocks: " + std::string(e.what())
      );
    }

    m_isInitialized = true;
  }

  void OpenGlDataBlockManager::upload(
    dataBlockType::Type dataBlockType,
    const void* data
  )
  {
    OpenGlDataBlock* dataBlock = getData(dataBlockType);
    if (dataBlock)
      dataBlock->upload(data);
  }

  bool OpenGlDataBlockManager::shouldTransposeMatrices() const
  {
    return true;
  }

  void OpenGlDataBlockManager::bind(dataBlockType::Type dataBlockType)
  {
    OpenGlDataBlock* dataBlock = getData(dataBlockType);
    if (dataBlock)
      dataBlock->bind(static_cast<UInt32>(dataBlockType));
  }

  void OpenGlDataBlockManager::bind(
    dataBlockType::Type dataBlockType,
    UInt32 bindingIndex
  )
  {
    OpenGlDataBlock* dataBlock = getData(dataBlockType);
    if (dataBlock)
      dataBlock->bind(bindingIndex);
  }

  void OpenGlDataBlockManager::destroy()
  {
    for (auto& [type, dataBlock] : m_dataBlocks)
    {
      if (dataBlock && dataBlock->isValid())
      {
        dataBlock->destroy();
        dataBlock.reset();
      }
    }
    m_dataBlocks.clear();
  }

  OpenGlDataBlock* OpenGlDataBlockManager::getData(dataBlockType::Type dataBlockType)
  {
    auto item = m_dataBlocks.find(dataBlockType);
    if (item == m_dataBlocks.end() || !item->second || !item->second->isValid())
    {
      throw RuntimeErrorException(
        "Data block of type " + std::to_string(dataBlockType) + " is not initialized or invalid."
      );
    }
    return item->second.get();
  }
}

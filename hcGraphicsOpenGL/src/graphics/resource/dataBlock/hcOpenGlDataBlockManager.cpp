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
    dataBlockStructure::MaterialHair materialHairInitData;
    dataBlockStructure::MaterialPBR materialPBRInitData;
    dataBlockStructure::Scene sceneInitData;

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

      UniquePtr<OpenGlDataBlock> materialPBR = MakeUnique<OpenGlDataBlock>();
      materialPBR->initialize(&materialPBRInitData, sizeof(materialPBRInitData));

      UniquePtr<OpenGlDataBlock> materialHair = MakeUnique<OpenGlDataBlock>();
      materialHair->initialize(&materialHairInitData, sizeof(materialHairInitData));

      UniquePtr<OpenGlDataBlock> scene = MakeUnique<OpenGlDataBlock>();
      scene->initialize(&sceneInitData, sizeof(sceneInitData));

      if (!camera->isValid() || !lights->isValid() || !lightShadows->isValid()
        || !object->isValid() || !lightViewProjection->isValid()
        || !materialUnlit->isValid() || !materialPBR->isValid()
        || !materialHair->isValid() || !scene->isValid())
      {
        throw RuntimeErrorException("Failed to initialize one or more OpenGL data blocks.");
      }

      m_dataBlocks[dataBlockType::Camera] = std::move(camera);
      m_dataBlocks[dataBlockType::Lights] = std::move(lights);
      m_dataBlocks[dataBlockType::LightShadows] = std::move(lightShadows);
      m_dataBlocks[dataBlockType::Object] = std::move(object);
      m_dataBlocks[dataBlockType::LightViewProjection] = std::move(lightViewProjection);
      m_dataBlocks[dataBlockType::MaterialUnlit] = std::move(materialUnlit);
      m_dataBlocks[dataBlockType::MaterialHair] = std::move(materialHair);
      m_dataBlocks[dataBlockType::MaterialPBR] = std::move(materialPBR);
      m_dataBlocks[dataBlockType::Scene] = std::move(scene);
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
    OpenGlDataBlock* dataBlock = getDataBlock(dataBlockType);
    if (dataBlock != nullptr)
      dataBlock->upload(data);
    else
      createDataBlock(dataBlockType, data);
  }

  bool OpenGlDataBlockManager::shouldTransposeMatrices() const
  {
    return true;
  }

  void OpenGlDataBlockManager::bind(dataBlockType::Type dataBlockType)
  {
    OpenGlDataBlock* dataBlock = getDataBlock(dataBlockType);
    if (dataBlock != nullptr)
      dataBlock->bind(static_cast<UInt32>(dataBlockType));
    else
      throw RuntimeErrorException(
        String::Format(
          "Data block of type: %d is not initialized or invalid.",
          static_cast<Int32>(dataBlockType)
        )
      );
  }

  void OpenGlDataBlockManager::bind(
    dataBlockType::Type dataBlockType,
    UInt32 bindingIndex
  )
  {
    OpenGlDataBlock* dataBlock = getDataBlock(dataBlockType);
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

  OpenGlDataBlock* OpenGlDataBlockManager::getDataBlock(dataBlockType::Type dataBlockType)
  {
    auto item = m_dataBlocks.find(dataBlockType);
    if (item == m_dataBlocks.end() || !item->second || !item->second->isValid())
      return nullptr;
    return item->second.get();
  }

  void OpenGlDataBlockManager::createDataBlock(
    dataBlockType::Type dataBlockType,
    const void* initialData
  )
  {
    SizeT dataSize = dataBlockType::GetDataBlockSize(dataBlockType);
    UniquePtr<OpenGlDataBlock> dataBlock = nullptr;

    try
    {
      dataBlock = MakeUnique<OpenGlDataBlock>();
      dataBlock->initialize(initialData, dataSize);
      if (!dataBlock->isValid())
      {
        throw RuntimeErrorException(
          String::Format(
            "Failed to initialize data block of type %d.",
            static_cast<Int32>(dataBlockType)
          )
        );
      }
    }
    catch (const Exception& e)
    {
      throw RuntimeErrorException(
        String::Format(
          "Failed to create data block of type %d: %s",
          static_cast<Int32>(dataBlockType),
          e.what()
        )
      );
    }

    if (dataBlock != nullptr && dataBlock->isValid())
      m_dataBlocks[dataBlockType] = std::move(dataBlock);
  }
}

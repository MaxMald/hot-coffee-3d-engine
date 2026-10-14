#include "hc/scene/skybox/hcSkybox.h"

#include "hc/serialization/hcFileFormats.h"
#include "hc/assets/image/hcImage.h"
#include "hc/assets/hcIAssetManager.h"
#include "hc/graphics/resource/cubeMap/hcICubeMap.h"
#include "hc/graphics/resource/cubeMap/hcCubeMapFactory.h"
#include "hc/graphics/hcIGraphicsManager.h"

namespace hc
{
  // --------------------- Skybox Settings Struct Implementation -------------------------

  static constexpr UInt32 SKYBOX_SETTINGS_VERSION = 1;

  void SkyboxSettings::serialize(io::BinaryWriter& writer) const
  {
    writer.startWritingObject(0, SKYBOX_SETTINGS_VERSION);
    writer.writePath(sourcePath);
    cubeMapGeneratorSettings.serialize(writer);
    diffuseIrradianceCubeMapGeneratorSettings.serialize(writer);
    writer.finishWritingObject();
  }

  void SkyboxSettings::deserialize(io::BinaryReader& reader)
  {
    clear();

    io::ObjectHeader header = reader.startReadingObject();
    UInt32 version = header.version;
    if (version >= 1)
    {
      sourcePath = reader.readPath();
      cubeMapGeneratorSettings.deserialize(reader);
      diffuseIrradianceCubeMapGeneratorSettings.deserialize(reader);
    }
    reader.finishReadingObject();
  }

  void SkyboxSettings::clear()
  {
    sourcePath.clear();
    cubeMapGeneratorSettings.clear();
    diffuseIrradianceCubeMapGeneratorSettings.clear();
  }

  // ------------------------- Skybox Class Implementation -----------------------------

  Skybox::Skybox() :
    m_cubeMap(nullptr),
    m_diffuseIrradianceCubeMap(nullptr)
  {}

  Skybox::~Skybox()
  {
    destroy();
  }

  void Skybox::initialize(
    SharedPtr<ICubeMap> cubeMap,
    SharedPtr<ICubeMap> diffuseIrradianceCubeMap
  )
  {
    if (cubeMap == nullptr || !cubeMap->isValid())
      throw RuntimeErrorException("Skybox::initialize: CubeMap is invalid.");
    if (diffuseIrradianceCubeMap == nullptr || !diffuseIrradianceCubeMap->isValid())
      throw RuntimeErrorException("Skybox::initialize: DiffuseIrradianceCubeMap is invalid.");

    m_cubeMap = cubeMap;
    m_diffuseIrradianceCubeMap = diffuseIrradianceCubeMap;
  }

  void Skybox::initialize(
    IAssetManager& assetManager,
    IGraphicsManager& graphicsManager,
    SkyboxSettings& settings
  )
  {
    destroy();

    if (settings.sourcePath.empty())
      throw InvalidArgumentException("Skybox::initialize: Source path is empty.");
    Path resolvedPath = settings.sourcePath;

    if (settings.sourcePath.isRelative() && !assetManager.hasRootPath())
      throw RuntimeErrorException("Skybox::initialize: Source path is relative but asset manager has no root path set.");

    if (settings.sourcePath.isRelative())
      resolvedPath = settings.sourcePath.toAbsolute(assetManager.getRootPath());

    IImageAssetManager& imageAssetManager = assetManager.getImageAssetManager();

    bool isCubeMapDescriptor = false;
    Path extension = resolvedPath.extension().toLowercase();
    if (extension == serialization::fileFormat::CubeMapDescriptor::FILE_EXTENSION)
      isCubeMapDescriptor = true;
    else if (imageAssetManager.isSupportedImage(resolvedPath))
      isCubeMapDescriptor = false;
    else
    {
      throw RuntimeErrorException(
        String::Format("Skybox::initialize: source path '%s' is neither a supported image nor a cube map descriptor.",
          resolvedPath.toString().c_str()
        )
      );
    }

    SharedPtr<ICubeMap> cubeMap = nullptr;
    try
    {
      if (isCubeMapDescriptor)
      {
        cubeMap = CubeMapFactory::CreateFromDescriptor(
          resolvedPath, assetManager, graphicsManager
        );
      }
      else
      {
        cubeMap = CubeMapFactory::CreateFromEquirectangularImage(
          resolvedPath, settings.cubeMapGeneratorSettings, assetManager, graphicsManager
        );
      }
    }
    catch (const Exception& ex)
    {
      destroy();
      throw RuntimeErrorException(
        "Skybox::initialize: Failed to create cube map from descriptor at path: " + resolvedPath.toString() +
        ". Error: " + String(ex.what())
      );
    }

    SharedPtr<ICubeMap> diffuseIrradianceCubeMap = nullptr;
    try
    {
      graphics::generators::IDiffuseIrradianceCubeMapGenerator& diffuseIrradianceGenerator =
        graphicsManager.getDiffuseIrradianceCubeMapGenerator();

      diffuseIrradianceCubeMap = diffuseIrradianceGenerator.generate(
        *cubeMap, settings.diffuseIrradianceCubeMapGeneratorSettings
      );
    }
    catch (const Exception& ex)
    {
      destroy();
      throw RuntimeErrorException(
        "Skybox::initialize: Failed to generate diffuse irradiance cube map from cube map at path: " + resolvedPath.toString() +
        ". Error: " + String(ex.what())
      );
    }

    initialize(cubeMap, diffuseIrradianceCubeMap);
  }

  bool Skybox::isValid() const
  {
    return m_cubeMap != nullptr && m_cubeMap->isValid()
      && m_diffuseIrradianceCubeMap != nullptr && m_diffuseIrradianceCubeMap->isValid();
  }

  void Skybox::clear()
  {
    m_cubeMap.reset();
    m_diffuseIrradianceCubeMap.reset();
  }

  void Skybox::destroy()
  {
    m_cubeMap.reset();
    m_diffuseIrradianceCubeMap.reset();
  }
}

#include "hc/assets/image/hcImageAssetManager.h"
#include "stb_image.h"

namespace hc
{
  static constexpr SizeT NUM_CHANNELS = 4;

  SharedPtr<Image> ImageAssetManager::load(const Path& path)
  {
    if (isLoaded(path))
      return m_loadedImages.at(path);

    if (!path.exists())
      return nullptr;

    bool isHDR = stbi_is_hdr(path.toString().c_str());
    if (isHDR)
      return loadHDRImage(path);
    else
      return loadLDRImage(path);
  }

  SharedPtr<Image> ImageAssetManager::get(const Path& path) const
  {
    if (isLoaded(path))
      return m_loadedImages.at(path);

    throw RuntimeErrorException("Image asset not loaded: " + path.toString());
  }

  bool ImageAssetManager::isLoaded(const Path& path) const
  {
    return m_loadedImages.find(path) != m_loadedImages.end();
  }

  SizeT ImageAssetManager::size() const
  {
    return m_loadedImages.size();
  }

  void ImageAssetManager::getAllLoadedAssets(
    Vector<SharedPtr<Image>>& outAssets
  ) const
  {
    outAssets.clear();
    for (const auto& pair : m_loadedImages)
      outAssets.push_back(pair.second);
  }

  void ImageAssetManager::clear()
  {
    m_loadedImages.clear();
  }

  SharedPtr<Image> ImageAssetManager::loadHDRImage(const Path& path)
  {
    Int32 width, height, channels;
    float* data = stbi_loadf(
      path.toString().c_str(),
      &width,
      &height,
      &channels,
      STBI_default
    );

    if (!data)
      return nullptr;

    textureFormatType::Type format = textureFormatType::RGBA32F;
    if (channels == 3)
      format = textureFormatType::RGB32F;
    else if (channels == 4)
      format = textureFormatType::RGBA32F;
    else
      throw RuntimeErrorException(
        String::Format(
          "Unsupported number of channels (%d) in HDR image: %s",
          channels,
          path.toString().c_str()
        )
      );

    SizeT bufferSize = static_cast<SizeT>(width)
      * static_cast<SizeT>(height)
      * static_cast<SizeT>(channels)
      * sizeof(float);

    BufferByte buffer(bufferSize);
    buffer.initialize(reinterpret_cast<Byte*>(data), bufferSize);
    stbi_image_free(data);

    SharedPtr<Image> image = MakeShared<Image>(
      path,
      static_cast<UInt32>(width),
      static_cast<UInt32>(height),
      format,
      colorSpaceType::Linear,
      std::move(buffer)
    );

    m_loadedImages[path] = image;
    return image;
  }

  SharedPtr<Image> ImageAssetManager::loadLDRImage(const Path& path)
  {
    Int32 width, height, channels;
    stbi_uc* data = stbi_load(
      path.toString().c_str(),
      &width,
      &height,
      &channels,
      STBI_default
    );

    if (!data)
      return nullptr;

    textureFormatType::Type format = textureFormatType::RGBA8;
    if (channels == 1)
      format = textureFormatType::R8;
    else if (channels == 2)
      format = textureFormatType::RG8;
    else if (channels == 3)
      format = textureFormatType::RGB8;

    SizeT bufferSize = static_cast<SizeT>(width)
      * static_cast<SizeT>(height)
      * static_cast<SizeT>(channels);

    BufferByte buffer(bufferSize);
    buffer.initialize(reinterpret_cast<Byte*>(data), bufferSize);

    stbi_image_free(data);

    SharedPtr<Image> image = MakeShared<Image>(
      path,
      static_cast<UInt32>(width),
      static_cast<UInt32>(height),
      format,
      colorSpaceType::SRGB,
      std::move(buffer)
    );

    m_loadedImages[path] = image;
    return image;
  }
}

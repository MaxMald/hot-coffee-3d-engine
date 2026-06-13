#include "hc/assets/image/hcImageAssetManager.h"
#include "stb_image.h"

namespace hc
{
  static constexpr SizeT NUM_CHANNELS = 4;

  SharedPtr<Image> ImageAssetManager::load(const Path& path)
  {
    if (isLoaded(path))
      return m_loadedImages.at(path);

    // TODO
    //
    // Method should determine the color format and color space of the loaded image. For
    // now, we will assume all loaded images are in RGBA8 format and sRGB color space, but
    // this may not always be the case.

    Int32 width = 0;
    Int32 height = 0;
    Int32 channels = 0;
    Byte* data = nullptr;
    SharedPtr<Image> image = nullptr;

    try
    {
      data = stbi_load(
        path.string().c_str(),
        reinterpret_cast<int*>(&width),
        reinterpret_cast<int*>(&height),
        reinterpret_cast<int*>(&channels),
        STBI_rgb_alpha
      );

      if (!data)
        return nullptr;

      SizeT bufferSize = static_cast<SizeT>(width)
        * static_cast<SizeT>(height)
        * NUM_CHANNELS;

      BufferByte buffer(bufferSize);
      buffer.initialize(data, bufferSize);

      image = MakeShared<Image>(
        path,
        static_cast<UInt32>(width),
        static_cast<UInt32>(height),
        colorFormatType::RGBA8,
        colorSpaceType::SRGB,
        std::move(buffer)
      );
    }
    catch (const Exception& e)
    {
      if (data)
        stbi_image_free(data);
      return nullptr;
    }

    stbi_image_free(data);
    m_loadedImages[path] = image;
    return image;
  }

  SharedPtr<Image> ImageAssetManager::get(const Path& path) const
  {
    if (isLoaded(path))
      return m_loadedImages.at(path);

    throw RuntimeErrorException("Image asset not loaded: " + path.string());
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
}

#include "hc/hcImageLoader.h"

#include "hc/hcImage.h"
#include "stb_image.h"

namespace hc
{
  static constexpr SizeT NUM_CHANNELS = 4;

  ImageLoader::ImageLoader()
  {
  }

  ImageLoader::~ImageLoader()
  {
  }

  SharedPtr<Image> ImageLoader::load(const Path& filePath)
  {
    Int32 width = 0;
    Int32 height = 0;
    Int32 channels = 0;

    Byte* data = stbi_load(
      filePath.string().c_str(),
      reinterpret_cast<int*>(&width),
      reinterpret_cast<int*>(&height),
      reinterpret_cast<int*>(&channels),
      STBI_rgb_alpha
    );

    if (!data)
      return nullptr;

    SizeT bufferSize = static_cast<SizeT>(width) * static_cast<SizeT>(height) * NUM_CHANNELS;
    BufferByte buffer(bufferSize);
    buffer.initialize(data, bufferSize);

    SharedPtr<Image> image = MakeShared<Image>();
    image->initialize(
      static_cast<UInt32>(width),
      static_cast<UInt32>(height),
      std::move(buffer)
    );

    stbi_image_free(data);
    return image;
  }
}

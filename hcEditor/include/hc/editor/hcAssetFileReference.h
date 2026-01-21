#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  /**
   * @brief Associates an asset with its corresponding file path.
   *
   * @tparam T The asset type.
   */
  template<typename T>
  class AssetFileReference
  {
  public:
    /**
     * @brief Default constructor.
     *
     * Initializes the file path as an empty string and the asset as nullptr.
     */
    AssetFileReference();

    /**
     * @brief Constructs an AssetFileReference with a file path and asset.
     *
     * @param filePath The file path associated with the asset.
     * @param asset The shared pointer to the asset.
     */
    AssetFileReference(const Path& filePath, const SharedPtr<T>& asset);

    /**
     * @brief Copy constructor.
     *
     * @param other The other AssetFileReference to copy from.
     */
    AssetFileReference(const AssetFileReference& other);

    /**
     * @brief Move constructor.
     *
     * @param other The other AssetFileReference to move from.
     */
    AssetFileReference(AssetFileReference&& other) noexcept;

    /**
     * @brief Copy assignment operator.
     *
     * @param other The other AssetFileReference to copy from.
     * @return Reference to this object.
     */
    AssetFileReference& operator=(const AssetFileReference& other);

    /**
     * @brief Move assignment operator.
     *
     * @param other The other AssetFileReference to move from.
     * @return Reference to this object.
     */
    AssetFileReference& operator=(AssetFileReference&& other) noexcept;

    /**
     * @brief Destructor.
     */
    ~AssetFileReference();

    /**
     * @brief Gets the file path associated with the asset.
     * 
     * @return const Path& The file path.
     */
    const Path& getFilePath() const;

    /**
     * @brief Sets the file path associated with the asset.
     * 
     * @param filePath The new file path.
     */
    void setFilePath(const Path& filePath);

    /**
     * @brief Gets the shared pointer to the asset.
     * 
     * @return SharedPtr<T> The asset.
     */
    SharedPtr<T> getAsset() const;

    /**
     * @brief Sets the shared pointer to the asset.
     * 
     * @param asset The new asset.
     */
    void setAsset(const SharedPtr<T>& asset);

  private:
    Path m_filePath;
    SharedPtr<T> m_asset = nullptr;
  };

  template<typename T>
  AssetFileReference<T>::AssetFileReference()
    : m_filePath(), m_asset(nullptr)
  {
  }

  template<typename T>
  AssetFileReference<T>::AssetFileReference(
    const Path& filePath,
    const SharedPtr<T>& asset
  )
    : m_filePath(filePath), m_asset(asset)
  {
  }

  template<typename T>
  AssetFileReference<T>::AssetFileReference(const AssetFileReference& other)
    : m_filePath(other.m_filePath), m_asset(other.m_asset)
  {
  }

  template<typename T>
  AssetFileReference<T>::AssetFileReference(AssetFileReference&& other) noexcept
    : m_filePath(std::move(other.m_filePath)), m_asset(std::move(other.m_asset))
  {
  }

  template<typename T>
  AssetFileReference<T>& AssetFileReference<T>::operator=(const AssetFileReference& other)
  {
    if (this != &other)
    {
      m_filePath = other.m_filePath;
      m_asset = other.m_asset;
    }
    return *this;
  }

  template<typename T>
  AssetFileReference<T>& AssetFileReference<T>::operator=(AssetFileReference&& other) noexcept
  {
    if (this != &other)
    {
      m_filePath = std::move(other.m_filePath);
      m_asset = std::move(other.m_asset);
    }
    return *this;
  }

  template<typename T>
  AssetFileReference<T>::~AssetFileReference() = default;

  template<typename T>
  const Path& AssetFileReference<T>::getFilePath() const
  {
    return m_filePath;
  }

  template<typename T>
  void AssetFileReference<T>::setFilePath(const Path& filePath)
  {
    m_filePath = filePath;
  }

  template<typename T>
  SharedPtr<T> AssetFileReference<T>::getAsset() const
  {
    return m_asset;
  }

  template<typename T>
  void AssetFileReference<T>::setAsset(const SharedPtr<T>& asset)
  {
    m_asset = asset;
  }
}

#pragma once

#include "hc/assets/hcAsset.h"
#include "hc/graphics/hcGraphicsCommons.h"

namespace hc
{
  namespace assets::materialDescriptor
  {
    struct UnlitData : public hc::io::ISerializable
    {
      Path textureImagePath;        ///< The path to the texture used by the unlit material
      Color color = Color::White(); ///< The base color of the unlit material

      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();
    };

    struct BlinnPhongData : public hc::io::ISerializable
    {
      Path diffuseImagePath;        ///< The path to the diffuse texture
      Path normalImagePath;         ///< The path to the normal texture
      Path specularImagePath;       ///< The path to the specular texture
      Color color = Color::White(); ///< The diffuse color of the Blinn-Phong material
      float shininess = 32.0f;      ///< The shininess factor for specular highlights

      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();
    };

    struct HairData : public hc::io::ISerializable
    {
      Path albedoImagePath;                           ///< The path to the albedo texture for hair
      Path normalImagePath;                           ///< The path to the normal texture for hair
      Path specularImagePath;                         ///< The path to the specular texture for hair
      Color color = Color::White();                   ///< The base color of the hair material
      Color specularPrimaryColor = Color::White();    ///< The primary specular color for hair shading
      Color specularSecondaryColor = Color::White();  ///< The secondary specular color for hair shading
      float shininess = 32.0f;                        ///< The shininess factor for specular highlights
      float specularPrimaryShift = 0.0f;              ///< The shift of the primary specular highlight
      float specularSecondaryShift = 0.0f;            ///< The shift of the secondary specular highlight
      float specularWidth = 1.0f;                     ///< The width of the specular highlight
      float specularStrength = 1.0f;                  ///< The strength of the specular highlight

      void serialize(io::BinaryWriter& writer) const override;
      void deserialize(io::BinaryReader& reader) override;
      void clear();
    };
  }

  using MaterialDescriptorVariant = std::variant<
    assets::materialDescriptor::UnlitData,
    assets::materialDescriptor::BlinnPhongData,
    assets::materialDescriptor::HairData
  >;

  /**
   * Abstract base class for material descriptors.
   *
   * Defines the interface for material descriptors that describe
   * material properties and associated resources. Implementations
   * provide specific material types with their shading models and
   * texture references.
   */
  class HC_CORE_EXPORT AMaterialDescriptor : public Asset, public io::ISerializable
  {
  public:
    MaterialDescriptorVariant variantData;  ///< Variant data holding specific material properties based on the material type 
    String name;                            ///< The name of the material descriptor
    float alphaCutoutThreshold;             ///< The threshold for alpha cutout rendering (used in AlphaCutout mode)
    bool doubleSided;                       ///< Indicates whether the material is double-sided (renders both front and back faces)
    materialRenderMode::Type renderMode;    ///< The render mode of the material (e.g., Opaque, Transparent)

    AMaterialDescriptor();
    AMaterialDescriptor(const Path& path);
    AMaterialDescriptor(materialType::Type _type, const Path& _path = "");
    virtual ~AMaterialDescriptor() = default;

    /**
     * Serializes the material descriptor to a binary format.
     *
     * This method writes the state of the material descriptor to the provided
     * BinaryWriter. Derived classes should implement their specific serialization
     * logic in the onSerialization method.
     *
     * @param writer The BinaryWriter to use for serialization.
     */
    virtual void serialize(io::BinaryWriter& writer) const override;

    /**
     * Deserializes the material descriptor from a binary format.
     *
     * This method reads the state of the material descriptor from the provided
     * BinaryReader. Derived classes should implement their specific deserialization
     * logic in the onDeserialization method.
     *
     * @param reader The BinaryReader to use for deserialization.
     */
    virtual void deserialize(io::BinaryReader& reader) override;

    /**
     * Gets the material type used by this material descriptor.
     *
     * @return The material type identifier (e.g., Unlit, BlinnPhong, Hair)
     */
    inline materialType::Type getType() const
    {
      return type;
    }

    /**
     * Sets the material type for this material descriptor.
     *
     * @param newType The new material type to set (e.g., Unlit, BlinnPhong, Hair)
     */
    void setType(materialType::Type newType);

    /**
     * Retrieves a pointer to the specific material data if it matches the requested type.
     *
     * @tparam T The type of material data to retrieve.
     *
     * @return A pointer to the material data of type T if it matches; otherwise, nullptr.
     */
    template<typename T>
    inline const T* getIf() const
    {
      return std::get_if<T>(&variantData);
    }

    /**
     * Retrieves a pointer to the specific material data if it matches the requested type.
     * (non-const version)
     *
     * @tparam T The type of material data to retrieve.
     *
     * @return A pointer to the material data of type T if it matches; otherwise, nullptr.
     */
    template<typename T>
    inline T* getIf()
    {
      return std::get_if<T>(&variantData);
    }

    /**
     * Retrieves a pointer to the UnlitData if the material type is Unlit.
     *
     * @return A pointer to the UnlitData if the material type is Unlit; otherwise,
     * nullptr.
     */
    inline const assets::materialDescriptor::UnlitData* getUnlitData() const
    {
      return std::get_if<assets::materialDescriptor::UnlitData>(&variantData);
    }

    /**
     * Retrieves a pointer to the UnlitData if the material type is Unlit.
     *
     * @return A pointer to the UnlitData if the material type is Unlit; otherwise,
     * nullptr.
     */
    inline assets::materialDescriptor::UnlitData* getUnlitData()
    {
      return std::get_if<assets::materialDescriptor::UnlitData>(&variantData);
    }

    /**
     * Retrieves a pointer to the BlinnPhongData if the material type is BlinnPhong.
     *
     * @return A pointer to the BlinnPhongData if the material type is BlinnPhong;
     * otherwise, nullptr.
     */
    inline const assets::materialDescriptor::BlinnPhongData* getBlinnPhongData() const
    {
      return std::get_if<assets::materialDescriptor::BlinnPhongData>(&variantData);
    }

    /**
     * Retrieves a pointer to the BlinnPhongData if the material type is BlinnPhong.
     *
     * @return A pointer to the BlinnPhongData if the material type is BlinnPhong;
     * otherwise, nullptr.
     */
    inline assets::materialDescriptor::BlinnPhongData* getBlinnPhongData()
    {
      return std::get_if<assets::materialDescriptor::BlinnPhongData>(&variantData);
    }

    /**
     * Retrieves a pointer to the HairData if the material type is Hair.
     *
     * @return A pointer to the HairData if the material type is Hair; otherwise, nullptr.
     */
    inline const assets::materialDescriptor::HairData* getHairData() const
    {
      return std::get_if<assets::materialDescriptor::HairData>(&variantData);
    }

    /**
     * Retrieves a pointer to the HairData if the material type is Hair.
     *
     * @return A pointer to the HairData if the material type is Hair; otherwise, nullptr.
     */
    inline assets::materialDescriptor::HairData* getHairData()
    {
      return std::get_if<assets::materialDescriptor::HairData>(&variantData);
    }

    /**
     * Clears the material descriptor's state, resetting it to default values.
     */
    virtual void clear();

  private:
    materialType::Type type;  ///< The type of the material (e.g., Unlit, BlinnPhong, Hair)
  };
}

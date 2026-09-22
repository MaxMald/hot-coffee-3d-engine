#pragma once

#include "hc/editor/services/materialDrawer/hcIMaterialDrawer.h"
#include "hc/editor/views/projectFileDialog/hcProjectFileDialogView.h"

namespace hc::editor
{
  namespace materialDrawer
  {
    /**
     * @brief Context structure for loading textures in the material drawer.
     *
     * This structure holds the necessary information for loading a texture
     * for a specific material type T. It is used to pass context to the
     * texture loading callback functions.
     *
     * @tparam T The material type associated with the texture being loaded.
     */
    template<typename T>
    struct LoadTextureContext
    {
      T* material;                    ///< Pointer to the material of type T.
      textureType::Type textureType;  ///< The type of texture being loaded.
    };
  }

  /**
   * @brief Abstract base class template for material drawer implementations.
   *
   * ABaseMaterialDrawer provides a generic implementation of the IMaterialDrawer
   * interface for materials of type T. It handles type checking and delegates
   * the actual drawing logic to the derived class via the pure virtual onDraw
   * method.
   *
   * @tparam T The material type this drawer handles.
   */
  template<typename T>
  class ABaseMaterialDrawer : public IMaterialDrawer
  {
  public:
    virtual ~ABaseMaterialDrawer() = default;

    /**
     * @copydoc IMaterialDrawer::drawMaterial
     */
    void drawMaterial(IMaterial* material) final;

    /**
     * @copydoc IMaterialDrawer::drawMeshMaterial
     */
    void drawMeshMaterial(
      IMaterial* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) final;

  protected:
    ITextureManager& m_textureManager;

    /**
     * @brief Constructs an ABaseMaterialDrawer with the specified texture manager.
     *
     * @param textureManager Reference to the ITextureManager for managing textures.
     */
    ABaseMaterialDrawer(ITextureManager& textureManager)
      : m_textureManager(textureManager)
    {}

    /**
     * @brief Draws the material-specific properties in the editor UI.
     *
     * This pure virtual method must be implemented by derived classes to handle
     * the actual drawing logic for materials of type T.
     *
     * @param material Pointer to the material of type T.
     */
    virtual void onDraw(T* material) = 0;

    /**
     * @brief Draws the material properties for a specific material slot in the editor UI.
     *
     * This pure virtual method must be implemented by derived classes to handle the
     * actual drawing logic for materials of type T in a specific material slot.
     *
     * @param material Pointer to the material of type T.
     * @param slotIndex The index of the material slot being drawn.
     * @param projectFileDialogView Reference to the ProjectFileDialogView for file
     * selection.
     */
    virtual void onDrawMeshMaterial(
      T* material,
      Int32 slotIndex,
      ProjectFileDialogView& projectFileDialogView
    ) = 0;

    /**
     * @brief Handles the selection of a texture for the material.
     *
     * @param material Pointer to the material of type T.
     * @param textureType The type of texture being selected.
     * @param projectFileDialogView Reference to the ProjectFileDialogView for file
     * selection.
     */
    void onLoadTextureClicked(
      T* material,
      textureType::Type textureType,
      ProjectFileDialogView& projectFileDialogView
    );
  };

  template<typename T>
  void ABaseMaterialDrawer<T>::drawMaterial(IMaterial* material)
  {
    if (!material)
      return;

    T* typedMaterial = dynamic_cast<T*>(material);
    if (!typedMaterial)
    {
      LogService::Error(
        "Material type mismatch in drawer. Expected: " + String(typeid(T).name())
      );
      return;
    }

    onDraw(typedMaterial);
  }

  template<typename T>
  inline void ABaseMaterialDrawer<T>::drawMeshMaterial(
    IMaterial* material,
    Int32 slotIndex,
    ProjectFileDialogView& projectFileDialogView
  )
  {
    if (!material)
      return;

    T* typedMaterial = dynamic_cast<T*>(material);
    if (!typedMaterial)
    {
      LogService::Error(
        "Material type mismatch in drawer. Expected: " + String(typeid(T).name())
      );
      return;
    }

    onDrawMeshMaterial(typedMaterial, slotIndex, projectFileDialogView);
  }

  template<typename T>
  void ABaseMaterialDrawer<T>::onLoadTextureClicked(
    T* material,
    textureType::Type textureType,
    ProjectFileDialogView& projectFileDialogView
  )
  {
    projectFileDialogView.openImageFile(
      [this](const Path& path, void* ctx)
      {
        auto* loadContext = static_cast<materialDrawer::LoadTextureContext<T>*>(ctx);
        if (loadContext == nullptr)
        {
          LogService::Error("LoadTextureContext is null.");
          return;
        }

        if (loadContext->material == nullptr)
        {
          LogService::Error("Material in LoadTextureContext is null.");
          return;
        }

        try
        {
          SharedPtr<ITexture> texture = m_textureManager.createTextureFromFile(path);
          if (texture != nullptr)
            loadContext->material->setTexture(loadContext->textureType, texture);
        }
        catch (const std::exception& e)
        {
          LogService::Error("Failed to load texture: " + String(e.what()));
        }
      },
      [](void*) // Cancel callback
      {
      },
      [](void* ctx) // Destroy context callback
      {
        delete static_cast<materialDrawer::LoadTextureContext<T>*>(ctx);
      },
      new materialDrawer::LoadTextureContext<T>
      {
        material,
        textureType
      }
    );
  }
}

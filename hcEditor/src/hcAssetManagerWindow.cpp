#include "hc/editor/hcAssetManagerWindow.h"
#include <hc/hcHotCoffeeEngine.h>
#include <hc/hcAssetManager.h>
#include <hc/hcDependencyContainer.h>
#include <hc/hcImage.h>
#include <hc/hcMaterialDescriptor.h>
#include <hc/hcAssetFileExtensions.h>
#include <hc/hcLogService.h>
#include "hc/editor/hcEditorViewsManager.h"
#include "hc/editor/hcProjectFileSelectorView.h"
#include "imgui.h"

namespace hc::editor
{
  AssetManagerWindow::AssetManagerWindow() :
    AWindowView("Asset Manager", false)
  {
  }

  AssetManagerWindow::~AssetManagerWindow()
  {
  }

  void AssetManagerWindow::resolveDependencies(DependencyContainer& container)
  {
    container.resolve<EditorViewsManager>()->registerView(this);
    m_fileSelectorView = container.resolve<ProjectFileSelectorView>();

    m_allAssetExtensions.clear();
    assetFileExtensions::GetAllAssetExtensions(m_allAssetExtensions);
  }

  void AssetManagerWindow::onDraw()
  {
    drawAssetLoadingSection();
    ImGui::Separator();

    AssetManager& assetManager = HotCoffeeEngine::Instance().getAssetManager();
    AssetGroup<Image>& imageGroup = assetManager.getGroup<Image>();
    if (ImGui::CollapsingHeader("Images"))
    {
      for (const auto& [key, imagePtr] : imageGroup.getAll())
      {
        if (ImGui::TreeNode(key.c_str()))
        {
          String path = imagePtr->getPath().generic_string();
          ImGui::Text("Path: %s", path.c_str());
          ImGui::Text("Dimensions: %ux%u", imagePtr->getWidth(), imagePtr->getHeight());
          ImGui::TreePop();
        }
      }
    }

    AssetGroup<MaterialDescriptor>& materialGroup = assetManager.getGroup<MaterialDescriptor>();
    if (ImGui::CollapsingHeader("Materials"))
    {
      for (const auto& [key, materialPtr] : materialGroup.getAll())
      {
        if (ImGui::TreeNode(key.c_str()))
        {
          String path = materialPtr->getPath().generic_string();
          ImGui::Text("Path: %s", path.c_str());
          ImGui::TreePop();
        }
      }
    }
  }

  void AssetManagerWindow::drawAssetLoadingSection()
  {
    if (ImGui::Button("Load"))
    {
      m_fileSelectorView->openFileSelector(
        "Select Asset",
        m_allAssetExtensions,
        [this](const Path& selectedPath)
        {
          this->loadAssetFromPath(selectedPath);
        }
      );
    }
  }

  void AssetManagerWindow::loadAssetFromPath(const Path& path)
  {
    AssetManager& assetManager = HotCoffeeEngine::Instance().getAssetManager();
    if (assetFileExtensions::IsImageExtension(path))
    {
      if (!assetManager.load<Image>(path.generic_string(), path))
      {
        LogService::Error(
          String::Format(
            "Failed to load image asset from path: %s", 
            path.generic_string().c_str()
          )
        );
      }
    }
    else if (assetFileExtensions::IsMaterialDescriptorExtension(path))
    {
      if (!assetManager.load<MaterialDescriptor>(path.generic_string(), path))
      {
        LogService::Error(
          String::Format(
            "Failed to load material descriptor asset from path: %s", 
            path.generic_string().c_str()
          )
        );
      }
    }
    else
    {
      LogService::Warning(
        String::Format(
          "Unsupported asset type for path: %s", 
          path.generic_string().c_str()
        )
      );
    }
  }
}

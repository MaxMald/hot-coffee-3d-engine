#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class AssetManagerWindow;

  namespace assetManagerDrawersRegistry
  {
    void registerAssetGroupDrawers(
      AssetManagerWindow& assetManagerWindow,
      IAssetManager& assetManager
    );
  }
}

#pragma once

#include "hc/editor/hcEditorPrerequisites.h"

namespace hc::editor
{
  class IEditorSceneManagerListener
  {
  public:
    virtual ~IEditorSceneManagerListener() = default;
    virtual void onSceneOpened() = 0;
    virtual void onSceneClosed() = 0;

  protected:
    IEditorSceneManagerListener() = default;
  };
}

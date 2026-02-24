#pragma once

#include "hc/editor/views/windows/hcAWindowView.h"

namespace hc::editor
{
  /**
   * @brief Window view for displaying meshes in the editor.
   */
  class MeshManagerWindow : public AWindowView
  {
  public:
    /**
    * @brief Constructs a MeshManagerWindow.
    * 
    * @param meshManager Reference to the IMeshManager used to retrieve mesh
    * data.
    */
    MeshManagerWindow(IMeshManager& meshManager);
    virtual ~MeshManagerWindow() = default;

  private:
    IMeshManager& m_meshManager;

    void onDraw() override;
    void drawMeshInfo(const SharedPtr<IMesh>& mesh);
    void drawMaterialsInfo(const Vector<SharedPtr<IMaterial>>& materials);
    void drawSubMeshesInfo(const Vector<ModelSubMesh>& subMeshes);
  };
}

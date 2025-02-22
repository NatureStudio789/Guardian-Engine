#ifndef _GE_GUARDIANSCENEHIERARCHYPANEL_H_
#define _GE_GUARDIANSCENEHIERARCHYPANEL_H_
#include "GuardianPanel.h"
#include <ECS/Scene/GuardianScene.h>
#include <Application/ResourceSystem/GuardianResourceSystem.h>

namespace guardian
{
	class GuardianSceneHierarchyPanel : public GuardianPanel
	{
	public:
		GuardianSceneHierarchyPanel();
		GuardianSceneHierarchyPanel(GuardianScene* scene);
		GuardianSceneHierarchyPanel(const GuardianSceneHierarchyPanel& other);
		~GuardianSceneHierarchyPanel();

		void SetScene(GuardianScene* scene);

		void Render() override;

		GuardianUUID GetSelectedEntityId();
		const int GetCurrentOperation() const;

	private:
		bool RenderEntityNode(GuardianEntity* entity);
		void RenderEntityComponents(bool& openMeshBrowser, 
			bool& openTextureBrowser, int& textureIndex, bool& openMaterialBrowser);
		void RenderMeshBrowser(GString& meshName, bool& open);
		void RenderTextureBrowser(GString& textureName, bool& open);
		void RenderMaterialBrowser(GString& materialName, bool& open);

		GuardianScene* PanelScene;
		GuardianUUID SelectedEntityId;
		int CurrentOperation;
		GuardianTexture MeshFileIcon;

		friend class GuardianEditor;
	};
}

#endif
#ifndef _GE_GUARDIANSCENEHIERARCHYPANEL_H_
#define _GE_GUARDIANSCENEHIERARCHYPANEL_H_
#include "GuardianPanel.h"
#include <ECS/Scene/GuardianScene.h>

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

	private:
		bool RenderEntityNode(GuardianEntity* entity);
		void RenderEntityComponents();

		GuardianScene* PanelScene;
		GuardianUUID SelectedEntityId;

		friend class GuardianEditor;
	};
}

#endif
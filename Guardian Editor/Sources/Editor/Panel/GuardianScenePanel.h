#ifndef _GE_GUARDIANSCENEPANEL_H_
#define _GE_GUARDIANSCENEPANEL_H_
#include "GuardianPanel.h"
#include <Application/GuardianApplication.h>

namespace guardian
{

	class GuardianScenePanel : public GuardianPanel
	{
	public:
		GuardianScenePanel();
		GuardianScenePanel(GuardianScene* scene);
		GuardianScenePanel(const GuardianScenePanel& other);
		~GuardianScenePanel();

		void SetSelectedEntityId(GuardianUUID entityId);

		void Render() override;

	private:
		GuardianUUID SelectedEntityId;
		GuardianScene* PanelScene;
		GuardianTexture PlayButtonTexture;
		GuardianTexture StopButtonTexture;

	};
}

#endif
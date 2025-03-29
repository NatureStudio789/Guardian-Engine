#ifndef _GE_GUARDIANSCENEPANEL_H_
#define _GE_GUARDIANSCENEPANEL_H_
#include "GuardianPanel.h"
#include <Application/GuardianApplication.h>

namespace GE
{

	class GuardianScenePanel : public GuardianPanel
	{
	public:
		GuardianScenePanel();
		GuardianScenePanel(GuardianScene* scene);
		GuardianScenePanel(const GuardianScenePanel& other);
		~GuardianScenePanel();

		void SetSelectedEntityId(GuardianUUID entityId);
		void SetCurrentOperation(int operation);

		void Render() override;

	private:
		GuardianUUID SelectedEntityId;
		int CurrentOperation;
		GuardianScene* PanelScene;
		GuardianTexture PlayButtonTexture;
		GuardianTexture StopButtonTexture;

	};
}

#endif
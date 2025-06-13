#ifndef _GE_GUARDIANSCENEPANEL_H_
#define _GE_GUARDIANSCENEPANEL_H_
#include <Widget/Panel/GuardianPanel.h>
#include <Application/GuardianApplication.h>

namespace GE
{

	class GuardianScenePanel : public EUI::GuardianPanel
	{
	public:
		GuardianScenePanel();
		GuardianScenePanel(const GuardianScenePanel& other);
		~GuardianScenePanel();

		void SetSelectedEntityId(GuardianUUID entityId);
		void SetCurrentOperation(int operation);

		void Render() override;

	private:
		GuardianUUID SelectedEntityId;
		int CurrentOperation;
		GuardianTexture PlayButtonTexture;
		GuardianTexture StopButtonTexture;

	};
}

#endif
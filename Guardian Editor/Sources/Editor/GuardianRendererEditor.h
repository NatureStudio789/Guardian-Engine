#ifndef _GE_GUARDIANRENDEREREDITOR_H_
#define _GE_GUARDIANRENDEREREDITOR_H_
#include "GuardianConsoleEditor.h"

namespace GE
{
	class GuardianRendererEditor : public EUI::GuardianEditor
	{
	public:
		GuardianRendererEditor();
		GuardianRendererEditor(const GuardianRendererEditor&) = default;

		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<EUI::GuardianPanel> MainPanel;
		std::shared_ptr<EUI::GuardianButton> TestButton;
		std::shared_ptr<EUI::GuardianText> FPSText;
		std::shared_ptr<EUI::GuardianText> EventText;
	};
}

#endif
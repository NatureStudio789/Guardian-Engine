#ifndef _GE_GUARDIANPROJECTEDITOR_H_
#define _GE_GUARDIANPROJECTEDITOR_H_
#include "GuardianConsoleEditor.h"

namespace GE
{
	class GuardianProjectEditor : public EUI::GuardianEditor
	{
	public:
		GuardianProjectEditor();
		GuardianProjectEditor(const GuardianProjectEditor& ) = default;

		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<EUI::GuardianPanel> MainPanel;
		std::shared_ptr<EUI::GuardianText> Title;
	};
}

#endif
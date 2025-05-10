#ifndef _GE_GUARDIANCONSOLEEDITOR_H_
#define _GE_GUARDIANCONSOLEEDITOR_H_
#include "GuardianEditor.h"

namespace GE
{
	class GuardianConsoleEditor : public GuardianEditor
	{
	public:
		GuardianConsoleEditor() = default;
		GuardianConsoleEditor(const GuardianConsoleEditor&) = default;

		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<GuardianPanel> ConsolePanel;
		std::shared_ptr<GuardianText> ConsoleLog;
		std::shared_ptr<GuardianColumnLayout> ButtonLayout;
		std::shared_ptr<GuardianButton> ClearLogButton;
		std::shared_ptr<GuardianButton> ExportLogButton;
	};
}

#endif
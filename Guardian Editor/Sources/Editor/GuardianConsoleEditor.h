#ifndef _GE_GUARDIANCONSOLEEDITOR_H_
#define _GE_GUARDIANCONSOLEEDITOR_H_
#include <Editor/GuardianEditor.h>

namespace GE
{
	class GuardianConsoleEditor : public EUI::GuardianEditor
	{
	public:
		GuardianConsoleEditor();
		GuardianConsoleEditor(const GuardianConsoleEditor&) = default;

		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<EUI::GuardianPanel> ConsolePanel;
		std::shared_ptr<EUI::GuardianText> ConsoleLog;
		std::shared_ptr<EUI::GuardianColumnLayout> ButtonLayout;
		std::shared_ptr<EUI::GuardianButton> ClearLogButton;
		std::shared_ptr<EUI::GuardianButton> ExportLogButton;
	};
}

#endif
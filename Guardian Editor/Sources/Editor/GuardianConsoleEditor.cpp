#include "GuardianConsoleEditor.h"
#include <Graphics/Applicable/Texture/GuardianTexture.h>
#include <Application/FileDialog/GuardianFileDialog.h>

namespace GE
{
	GuardianConsoleEditor::GuardianConsoleEditor()
	{
		this->EditorName = "Console Editor";
	}

	void GuardianConsoleEditor::Initialize()
	{
		this->ConsolePanel = std::make_shared<EUI::GuardianPanel>();
		this->ConsolePanel->SetPanelName("Console");

		this->ConsoleLog = std::make_shared<EUI::GuardianText>();

		this->ButtonLayout = std::make_shared<EUI::GuardianColumnLayout>();
		this->ButtonLayout->SetColumnCount(2);

		this->ClearLogButton = std::make_shared<EUI::GuardianButton>(
			"Clear",
			[]()
			{
				GuardianLogger::ClearMessageList();
			}, GVector2{ 50.0f, 30.0f });

		this->ExportLogButton = std::make_shared<EUI::GuardianButton>(
			"Export",
			[]()
			{
				GString Path = GuardianFileDialog::SaveFile("Guardian Engine Log(*.glog)\0 * .glog\0");
				if (Path.find_last_of('.') == Path.npos)
				{
					Path += ".glog";
				}

				GuardianLogger::ExportCurrentLog(Path);
			}, GVector2{ 50.0f, 30.0f });

		this->ButtonLayout->AddWidgetToLayout(this->ClearLogButton);
		this->ButtonLayout->AddWidgetToLayout(this->ExportLogButton);

		this->ConsolePanel->AddWidgetToPanel(this->ButtonLayout);
		this->ConsolePanel->AddWidgetToPanel(this->ConsoleLog);

		this->AddWidgetToEditor(this->ConsolePanel);
	}

	void GuardianConsoleEditor::Update()
	{
		this->ConsoleLog->SetTextLabel(GuardianLogger::GetLog());
	}
}

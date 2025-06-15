#include "GuardianProjectEditor.h"

namespace GE
{
	GuardianProjectEditor::GuardianProjectEditor()
	{
		this->EditorName = "Project Editor";
	}

	void GuardianProjectEditor::Initialize()
	{
		this->MainPanel = std::make_shared<EUI::GuardianPanel>();
		this->MainPanel->SetPanelName("Project Creator");
		this->MainPanel->AddPanelFlags(EUI::GuardianPanel::GEPanelFlag_NoDecoration | 
			EUI::GuardianPanel::GEPanelFlag_NoScrollbar | EUI::GuardianPanel::GEPanelFlag_NoScrollWithMouse);

		this->Title = std::make_shared<EUI::GuardianText>("Project Creator");
		
		this->MainPanel->AddWidgetToPanel(this->Title);

		this->AddWidgetToEditor(this->MainPanel);
	}

	void GuardianProjectEditor::Update()
	{

	}
}

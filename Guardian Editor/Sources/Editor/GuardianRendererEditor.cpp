#include "GuardianRendererEditor.h"

namespace GE
{
	void GuardianRendererEditor::Initialize()
	{
		this->MainPanel = std::make_shared<GuardianPanel>();
		this->MainPanel->SetPanelName("Renderer");

		this->TestButton = std::make_shared<GuardianButton>("Clicked Me!", 
			[=]()
			{
				this->EventText->SetTextLabel("True!");
			}, 
			GVector2(50.0f, 20.0f));
		this->FPSText = std::make_shared<GuardianText>("FPS : 0");
		this->EventText = std::make_shared<GuardianText>("False!");

		this->AddWidgetToEditor(this->MainPanel);
		this->MainPanel->AddWidgetToPanel(this->TestButton);
		this->MainPanel->AddWidgetToPanel(this->FPSText);
		this->MainPanel->AddWidgetToPanel(this->EventText);
	}

	void GuardianRendererEditor::Update()
	{
		this->FPSText->SetTextLabel("FPS : " + std::to_string(GuardianTime::GetFPSCount()));
	}
}

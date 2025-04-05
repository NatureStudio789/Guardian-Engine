#include "GuardianEditor.h"

namespace GE
{
	void GuardianEditor::AddWidgetToEditor(std::shared_ptr<GuardianWidget> widget)
	{
		this->EditorWidgetList.push_back(widget);
	}

	void GuardianEditor::ClearEditorWidgets()
	{
		this->EditorWidgetList.clear();
	}

	void GuardianEditor::Update()
	{
		this->UpdateEditorWidgets();
	}

	void GuardianEditor::Render()
	{
		this->RenderEditorWidgets();
	}

	void GuardianEditor::RenderEditorWidgets()
	{
		for (int i = 0; i < (int)this->EditorWidgetList.size(); i++)
		{
			if (this->EditorWidgetList[i]->IsEnableRendering())
			{
				for (int j = 0; j < (int)this->EditorWidgetList[i]->GetWidgetStyleList().size(); j++)
				{
					GVector2 value = this->EditorWidgetList[i]->GetWidgetStyleList()[j].Value;
					const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->EditorWidgetList[i]->GetWidgetStyleList()[j].Index);
					if (info->Count == 1)
					{
						ImGui::PushStyleVar(
							(ImGuiStyleVar)this->EditorWidgetList[i]->GetWidgetStyleList()[j].Index, value.x);
					}
					else if (info->Count == 2)
					{
						ImGui::PushStyleVar(
							(ImGuiStyleVar)this->EditorWidgetList[i]->GetWidgetStyleList()[j].Index, ImVec2(value.x, value.y));
					}
				}

				this->EditorWidgetList[i]->Render();

				ImGui::PopStyleVar((int)this->EditorWidgetList[i]->GetWidgetStyleList().size());
			}
		}
	}

	void GuardianEditor::UpdateEditorWidgets()
	{
		for (int i = 0; i < (int)this->EditorWidgetList.size(); i++)
		{
			this->EditorWidgetList[i]->Update();
		}
	}
}

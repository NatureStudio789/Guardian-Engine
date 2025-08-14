#include "GEditor.h"

namespace GE
{
	namespace EUI
	{
		void GEditor::AddWidgetToEditor(std::shared_ptr<GWidget> widget)
		{
			this->EditorWidgetList.push_back(widget);
		}

		void GEditor::ClearEditorWidgets()
		{
			this->EditorWidgetList.clear();
		}

		void GEditor::Update()
		{
			this->UpdateEditorWidgets();
		}

		void GEditor::Render()
		{
			this->RenderEditorWidgets();
		}

		void GEditor::RenderEditorWidgets()
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

					for (int j = 0; j < (int)this->EditorWidgetList[i]->GetWidgetColorList().size(); j++)
					{
						GVector4 value = this->EditorWidgetList[i]->GetWidgetColorList()[j].Value;
						ImGui::PushStyleColor((ImGuiCol)this->EditorWidgetList[i]->GetWidgetColorList()[j].Index,
							ImVec4(value.x, value.y, value.z, value.w));
					}

					this->EditorWidgetList[i]->Render();

					ImGui::PopStyleVar((int)this->EditorWidgetList[i]->GetWidgetStyleList().size());
					ImGui::PopStyleColor((int)this->EditorWidgetList[i]->GetWidgetColorList().size());
				}
			}
		}

		void GEditor::UpdateEditorWidgets()
		{
			for (int i = 0; i < (int)this->EditorWidgetList.size(); i++)
			{
				this->EditorWidgetList[i]->Update();
			}
		}

		const std::string& GEditor::GetEditorName() const noexcept
		{
			return this->EditorName;
		}
	}
}

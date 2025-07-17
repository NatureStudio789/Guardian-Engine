#include "GuardianPanel.h"

namespace GE
{
    namespace EUI
    {
        GuardianPanel::GuardianPanel(const std::string& name)
        {
            this->SetPanelName(name);
            this->PanelFlags = 0;
        }

        void GuardianPanel::SetPanelName(const std::string& name)
        {
            this->PanelName = name;
        }

        void GuardianPanel::AddWidgetToPanel(std::shared_ptr<GuardianWidget> widget)
        {
            this->PanelWidgetList.push_back(widget);
        }

        void GuardianPanel::ClearPanelWidgets()
        {
            this->PanelWidgetList.clear();
        }

        void GuardianPanel::AddPanelFlag(Flag flag)
        {
            this->PanelFlags |= flag;
        }

        void GuardianPanel::AddPanelFlags(int flags)
        {
            this->PanelFlags |= flags;
        }

        void GuardianPanel::Update()
        {
            this->UpdatePanelWidgets();
        }

        void GuardianPanel::Render()
        {
            this->RenderPanelWidgets();
        }

        const std::string& GuardianPanel::GetPanelName() const noexcept
        {
            return this->PanelName;
        }

        void GuardianPanel::RenderPanelWidgets()
        {
            ImGui::Begin(this->PanelName.c_str(), &this->EnableRendering, this->PanelFlags);

            this->CallWidgetEventProcessFunction();

            for (int i = 0; i < (int)this->PanelWidgetList.size(); i++)
            {
                if (this->PanelWidgetList[i]->IsEnableRendering())
                {
                    for (int j = 0; j < (int)this->PanelWidgetList[i]->GetWidgetStyleList().size(); j++)
                    {
                        GVector2 value = this->PanelWidgetList[i]->GetWidgetStyleList()[j].Value;
                        const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->PanelWidgetList[i]->GetWidgetStyleList()[j].Index);
                        if (info->Count == 1)
                        {
                            ImGui::PushStyleVar(
                                (ImGuiStyleVar)this->PanelWidgetList[i]->GetWidgetStyleList()[j].Index, value.x);
                        }
                        else if (info->Count == 2)
                        {
                            ImGui::PushStyleVar(
                                (ImGuiStyleVar)this->PanelWidgetList[i]->GetWidgetStyleList()[j].Index, ImVec2(value.x, value.y));
                        }
                    }

                    for (int j = 0; j < (int)this->PanelWidgetList[i]->GetWidgetColorList().size(); j++)
                    {
                        GVector4 value = this->PanelWidgetList[i]->GetWidgetColorList()[j].Value;
                        ImGui::PushStyleColor((ImGuiCol)this->PanelWidgetList[i]->GetWidgetColorList()[j].Index,
                            ImVec4(value.x, value.y, value.z, value.w));
                    }

                    this->PanelWidgetList[i]->Render();

                    ImGui::PopStyleVar((int)this->PanelWidgetList[i]->GetWidgetStyleList().size());
                    ImGui::PopStyleColor((int)this->PanelWidgetList[i]->GetWidgetColorList().size());
                }
            }

            ImGui::End();
        }

        void GuardianPanel::UpdatePanelWidgets()
        {
            for (int i = 0; i < (int)this->PanelWidgetList.size(); i++)
            {
                this->PanelWidgetList[i]->Update();
            }
        }
    }
}

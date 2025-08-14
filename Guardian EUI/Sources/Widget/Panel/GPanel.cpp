#include "GPanel.h"

namespace GE
{
    namespace EUI
    {
        GPanel::GPanel(const std::string& name)
        {
            this->SetPanelName(name);
            this->PanelFlags = 0;
        }

        void GPanel::SetPanelName(const std::string& name)
        {
            this->PanelName = name;
        }

        void GPanel::AddWidgetToPanel(std::shared_ptr<GWidget> widget)
        {
            this->PanelWidgetList.push_back(widget);
        }

        void GPanel::ClearPanelWidgets()
        {
            this->PanelWidgetList.clear();
        }

        void GPanel::AddPanelFlag(Flag flag)
        {
            this->PanelFlags |= flag;
        }

        void GPanel::AddPanelFlags(int flags)
        {
            this->PanelFlags |= flags;
        }

        void GPanel::Update()
        {
            this->UpdatePanelWidgets();
        }

        void GPanel::Render()
        {
            this->RenderPanelWidgets();
        }

        const std::string& GPanel::GetPanelName() const noexcept
        {
            return this->PanelName;
        }

        void GPanel::RenderPanelWidgets()
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

        void GPanel::UpdatePanelWidgets()
        {
            for (int i = 0; i < (int)this->PanelWidgetList.size(); i++)
            {
                this->PanelWidgetList[i]->Update();
            }
        }
    }
}

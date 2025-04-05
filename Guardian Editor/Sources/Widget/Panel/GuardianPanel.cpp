#include "GuardianPanel.h"

namespace GE
{
    void GuardianPanel::SetPanelName(const GString& name)
    {
        this->PanelName = name;
    }

    void GuardianPanel::AddWidgetToPanel(std::shared_ptr<GuardianWidget> widget)
    {
        this->PanelWidgetList.push_back(widget);
    }

    void GuardianPanel::ClearPanelWidgets()
    {

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

    const GString& GuardianPanel::GetPanelName() const noexcept
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

                this->PanelWidgetList[i]->Render();

                ImGui::PopStyleVar((int)this->PanelWidgetList[i]->GetWidgetStyleList().size());
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

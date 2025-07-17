#include "GuardianMenuBar.h"

namespace GE
{
    namespace EUI
    {
        void GuardianMenuBar::AddWidgetToMenuBar(std::shared_ptr<GuardianWidget> widget)
        {
            this->MenuBarWidgetList.push_back(widget);
        }

        void GuardianMenuBar::ClearMenuBarWidgets()
        {
            this->MenuBarWidgetList.clear();
        }

        void GuardianMenuBar::Update()
        {

        }

        void GuardianMenuBar::Render()
        {
            this->RenderMenuBarWidgets();
        }

        void GuardianMenuBar::RenderMenuBarWidgets()
        {
            if (ImGui::BeginMenuBar())
            {
                this->CallWidgetEventProcessFunction();

                for (int i = 0; i < (int)this->MenuBarWidgetList.size(); i++)
                {
                    if (this->MenuBarWidgetList[i]->IsEnableRendering())
                    {
                        for (int j = 0; j < (int)this->MenuBarWidgetList[i]->GetWidgetStyleList().size(); j++)
                        {
                            GVector2 value = this->MenuBarWidgetList[i]->GetWidgetStyleList()[j].Value;
                            const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->MenuBarWidgetList[i]->GetWidgetStyleList()[j].Index);
                            if (info->Count == 1)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->MenuBarWidgetList[i]->GetWidgetStyleList()[j].Index, value.x);
                            }
                            else if (info->Count == 2)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->MenuBarWidgetList[i]->GetWidgetStyleList()[j].Index, ImVec2(value.x, value.y));
                            }
                        }

                        for (int j = 0; j < (int)this->MenuBarWidgetList[i]->GetWidgetColorList().size(); j++)
                        {
                            GVector4 value = this->MenuBarWidgetList[i]->GetWidgetColorList()[j].Value;
                            ImGui::PushStyleColor((ImGuiCol)this->MenuBarWidgetList[i]->GetWidgetColorList()[j].Index,
                                ImVec4(value.x, value.y, value.z, value.w));
                        }

                        this->MenuBarWidgetList[i]->Render();

                        ImGui::PopStyleVar((int)this->MenuBarWidgetList[i]->GetWidgetStyleList().size());
                        ImGui::PopStyleColor((int)this->MenuBarWidgetList[i]->GetWidgetColorList().size());
                    }
                }

                ImGui::EndMenuBar();
            }
        }
    }
}

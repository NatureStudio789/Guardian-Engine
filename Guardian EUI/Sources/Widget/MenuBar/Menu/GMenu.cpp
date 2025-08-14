#include "GMenu.h"

namespace GE
{
    namespace EUI
    {
        void GMenu::SetMenuLabel(const std::string& label)
        {
            this->MenuLabel = label;
        }

        void GMenu::AddWidgetToMenu(std::shared_ptr<GWidget> widget)
        {
            this->MenuWidgetList.push_back(widget);
        }

        void GMenu::ClearMenuWidgets()
        {
            this->MenuWidgetList.clear();
        }

        void GMenu::Update()
        {

        }

        void GMenu::Render()
        {
            this->RenderMenuWidgets();
        }

        void GMenu::RenderMenuWidgets()
        {
            if (ImGui::BeginMenu(this->MenuLabel.c_str()))
            {
                this->CallWidgetEventProcessFunction();

                for (int i = 0; i < (int)this->MenuWidgetList.size(); i++)
                {
                    if (this->MenuWidgetList[i]->IsEnableRendering())
                    {
                        for (int j = 0; j < (int)this->MenuWidgetList[i]->GetWidgetStyleList().size(); j++)
                        {
                            GVector2 value = this->MenuWidgetList[i]->GetWidgetStyleList()[j].Value;
                            const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->MenuWidgetList[i]->GetWidgetStyleList()[j].Index);
                            if (info->Count == 1)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->MenuWidgetList[i]->GetWidgetStyleList()[j].Index, value.x);
                            }
                            else if (info->Count == 2)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->MenuWidgetList[i]->GetWidgetStyleList()[j].Index, ImVec2(value.x, value.y));
                            }
                        }

                        for (int j = 0; j < (int)this->MenuWidgetList[i]->GetWidgetColorList().size(); j++)
                        {
                            GVector4 value = this->MenuWidgetList[i]->GetWidgetColorList()[j].Value;
                            ImGui::PushStyleColor((ImGuiCol)this->MenuWidgetList[i]->GetWidgetColorList()[j].Index,
                                ImVec4(value.x, value.y, value.z, value.w));
                        }

                        this->MenuWidgetList[i]->Render();

                        ImGui::PopStyleVar((int)this->MenuWidgetList[i]->GetWidgetStyleList().size());
                        ImGui::PopStyleColor((int)this->MenuWidgetList[i]->GetWidgetColorList().size());
                    }
                }

                ImGui::EndMenu();
            }
        }
    }
}

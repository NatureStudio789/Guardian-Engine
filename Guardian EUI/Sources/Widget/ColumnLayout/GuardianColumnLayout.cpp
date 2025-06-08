#include "GuardianColumnLayout.h"

namespace GE
{
    namespace EUI
    {
        void GuardianColumnLayout::AddWidgetToLayout(std::shared_ptr<GuardianWidget> widget)
        {
            this->LayoutWidgetList.push_back(widget);
        }

        void GuardianColumnLayout::SetColumnCount(int count)
        {
            this->LayoutColumnCount = count;
        }

        void GuardianColumnLayout::ClearLayoutWidgets()
        {
            this->LayoutWidgetList.clear();
        }

        void GuardianColumnLayout::Update()
        {

        }

        void GuardianColumnLayout::Render()
        {
            this->RenderLayoutWidgets();
        }

        const int& GuardianColumnLayout::GetColumnCount() const noexcept
        {
            return this->LayoutColumnCount;
        }

        void GuardianColumnLayout::RenderLayoutWidgets()
        {
            ImGui::Columns(this->LayoutColumnCount);

            for (int i = 0; i < (int)this->LayoutWidgetList.size(); i++)
            {
                if (this->LayoutWidgetList[i]->IsEnableRendering())
                {
                    for (int j = 0; j < (int)this->LayoutWidgetList[i]->GetWidgetStyleList().size(); j++)
                    {
                        GVector2 value = this->LayoutWidgetList[i]->GetWidgetStyleList()[j].Value;
                        const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->LayoutWidgetList[i]->GetWidgetStyleList()[j].Index);
                        if (info->Count == 1)
                        {
                            ImGui::PushStyleVar(
                                (ImGuiStyleVar)this->LayoutWidgetList[i]->GetWidgetStyleList()[j].Index, value.x);
                        }
                        else if (info->Count == 2)
                        {
                            ImGui::PushStyleVar(
                                (ImGuiStyleVar)this->LayoutWidgetList[i]->GetWidgetStyleList()[j].Index, ImVec2(value.x, value.y));
                        }
                    }

                    for (int j = 0; j < (int)this->LayoutWidgetList[i]->GetWidgetColorList().size(); j++)
                    {
                        GVector4 value = this->LayoutWidgetList[i]->GetWidgetColorList()[j].Value;
                        ImGui::PushStyleColor((ImGuiCol)this->LayoutWidgetList[i]->GetWidgetColorList()[j].Index,
                            ImVec4(value.x, value.y, value.z, value.w));
                    }

                    this->LayoutWidgetList[i]->Render();

                    ImGui::PopStyleVar((int)this->LayoutWidgetList[i]->GetWidgetStyleList().size());
                    ImGui::PopStyleColor((int)this->LayoutWidgetList[i]->GetWidgetColorList().size());

                    ImGui::NextColumn();
                }
            }

            ImGui::Columns(1);
        }
    }
}

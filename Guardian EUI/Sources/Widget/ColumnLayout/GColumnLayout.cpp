#include "GColumnLayout.h"

namespace GE
{
    namespace EUI
    {
        void GColumnLayout::AddColumn(std::vector<std::shared_ptr<GWidget>> column)
        {
            this->ColumnList.push_back(column);
        }

        void GColumnLayout::SetColumnCount(int count)
        {
            this->LayoutColumnCount = count;
        }

        void GColumnLayout::ClearLayoutWidgets()
        {
            this->ColumnList.clear();
        }

        void GColumnLayout::Update()
        {

        }

        void GColumnLayout::Render()
        {
            this->RenderLayoutWidgets();
        }

        const int& GColumnLayout::GetColumnCount() const noexcept
        {
            return this->LayoutColumnCount;
        }

        void GColumnLayout::RenderLayoutWidgets()
        {
            ImGui::Columns(this->LayoutColumnCount, 0, false);

            for (int i = 0; i < (int)this->ColumnList.size(); i++)
            {
                for (int j = 0; j < (int)this->ColumnList[i].size(); j++)
                {
                    if (this->ColumnList[i][j]->IsEnableRendering())
                    {
                        for (int q = 0; q < (int)this->ColumnList[i][j]->GetWidgetStyleList().size(); q++)
                        {
                            GVector2 value = this->ColumnList[i][j]->GetWidgetStyleList()[q].Value;
                            const ImGuiDataVarInfo* info = ImGui::GetStyleVarInfo((ImGuiStyleVar)this->ColumnList[i][j]->GetWidgetStyleList()[q].Index);
                            if (info->Count == 1)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->ColumnList[i][j]->GetWidgetStyleList()[q].Index, value.x);
                            }
                            else if (info->Count == 2)
                            {
                                ImGui::PushStyleVar(
                                    (ImGuiStyleVar)this->ColumnList[i][j]->GetWidgetStyleList()[q].Index, ImVec2(value.x, value.y));
                            }
                        }

                        for (int q = 0; q < (int)this->ColumnList[i][j]->GetWidgetColorList().size(); q++)
                        {
                            GVector4 value = this->ColumnList[i][j]->GetWidgetColorList()[q].Value;
                            ImGui::PushStyleColor((ImGuiCol)this->ColumnList[i][j]->GetWidgetColorList()[q].Index,
                                ImVec4(value.x, value.y, value.z, value.w));
                        }

                        this->ColumnList[i][j]->Render();

                        ImGui::PopStyleVar((int)this->ColumnList[i][j]->GetWidgetStyleList().size());
                        ImGui::PopStyleColor((int)this->ColumnList[i][j]->GetWidgetColorList().size());
                    }
                }

                ImGui::NextColumn();
            }

            ImGui::Columns(1);
        }
    }
}

#include "GTreeNode.h"

namespace GE
{
	namespace EUI
	{
		GTreeNode::GTreeNode(const std::string& label, Flag flags)
		{
			this->SetTreeNodeLabel(label);
			this->SetTreeNodeFlags(flags);
		}

		void GTreeNode::SetTreeNodeLabel(const std::string& label)
		{
			this->TreeNodeLabel = label;
		}

		void GTreeNode::SetTreeNodeFlags(Flag flags)
		{
			this->TreeNodeFlags = flags;
		}

		void GTreeNode::AddWidgetToTreeNode(std::shared_ptr<GWidget> widget)
		{
			this->WidgetList.push_back(widget);
		}

		void GTreeNode::RemoveWidget(const GUUID& widgetId)
		{
			for (auto it = this->WidgetList.begin(); it != this->WidgetList.end(); it++)
			{
				if ((*it)->GetWidgetId() == widgetId)
				{
					this->WidgetList.erase(it);
					break;
				}
			}
		}

		void GTreeNode::RemoveWidget(std::shared_ptr<GWidget> widget)
		{
			for (auto it = this->WidgetList.begin(); it != this->WidgetList.end(); it++)
			{
				if ((*it) == widget)
				{
					this->WidgetList.erase(it);
					break;
				}
			}
		}

		void GTreeNode::ClearWidgets()
		{
			this->WidgetList.clear();
		}

		void GTreeNode::Update()
		{
			for (auto& widget : this->WidgetList)
			{
				widget->Update();
			}
		}

		void GTreeNode::Render()
		{
			bool IsOpened = ImGui::TreeNodeEx(this->TreeNodeLabel.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | this->TreeNodeFlags);

			this->CallWidgetEventProcessFunction();

			if (IsOpened)
			{
				for (auto& widget : this->WidgetList)
				{
					widget->Render();
				}

				ImGui::TreePop();
			}
		}

		const std::string& GTreeNode::GetTreeNodeLabel() const noexcept
		{
			return this->TreeNodeLabel;
		}
	}
}

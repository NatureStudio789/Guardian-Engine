#ifndef _GE_GTREENODE_H_
#define _GE_GTREENODE_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GTreeNode : public GWidget
		{
		public:
			enum Flag
			{
				GETreeNodeFlags_None = ImGuiTreeNodeFlags_None,
				GETreeNodeFlags_Selected = ImGuiTreeNodeFlags_Selected,
				GETreeNodeFlags_Framed = ImGuiTreeNodeFlags_Framed,
				GETreeNodeFlags_AllowOverlap = ImGuiTreeNodeFlags_AllowOverlap,
				GETreeNodeFlags_NoTreePushOnOpen = ImGuiTreeNodeFlags_NoTreePushOnOpen,
				GETreeNodeFlags_NoAutoOpenOnLog = ImGuiTreeNodeFlags_NoAutoOpenOnLog,
				GETreeNodeFlags_DefaultOpen = ImGuiTreeNodeFlags_DefaultOpen,
				GETreeNodeFlags_OpenOnDoubleClick = ImGuiTreeNodeFlags_OpenOnDoubleClick,
				GETreeNodeFlags_OpenOnArrow = ImGuiTreeNodeFlags_OpenOnArrow,
				GETreeNodeFlags_Leaf = ImGuiTreeNodeFlags_Leaf,
				GETreeNodeFlags_Bullet = ImGuiTreeNodeFlags_Bullet,
				GETreeNodeFlags_FramePadding = ImGuiTreeNodeFlags_FramePadding,
				GETreeNodeFlags_SpanAvailWidth = ImGuiTreeNodeFlags_SpanAvailWidth,
				GETreeNodeFlags_SpanFullWidth = ImGuiTreeNodeFlags_SpanFullWidth,
				GETreeNodeFlags_SpanTextWidth = ImGuiTreeNodeFlags_SpanTextWidth,
				GETreeNodeFlags_SpanAllColumns = ImGuiTreeNodeFlags_SpanAllColumns,
				GETreeNodeFlags_NavLeftJumpsBackHere = ImGuiTreeNodeFlags_NavLeftJumpsBackHere,
				GETreeNodeFlags_CollapsingHeader = ImGuiTreeNodeFlags_CollapsingHeader,
			};

		public:
			GTreeNode() = default;
			GTreeNode(const std::string& label, int flags = 0);
			GTreeNode(const GTreeNode&) = default;
			~GTreeNode() = default;

			void SetTreeNodeLabel(const std::string& label);
			void SetTreeNodeFlags(int flags);

			void AddWidgetToTreeNode(std::shared_ptr<GWidget> widget);
			void RemoveWidget(const GUUID& widgetId);
			void RemoveWidget(std::shared_ptr<GWidget> widget);
			void ClearWidgets();

			void Update() override;
			
			void Render() override;

			const std::string& GetTreeNodeLabel() const noexcept;

		private:
			std::string TreeNodeLabel;
			int TreeNodeFlags;

			std::vector<std::shared_ptr<GWidget>> WidgetList;
		};
	}
}

#endif
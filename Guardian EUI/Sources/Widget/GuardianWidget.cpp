#include "GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		void GuardianWidget::EnableWidgetRendering()
		{
			this->EnableRendering = true;
		}

		void GuardianWidget::DisableWidgetRendering()
		{
			this->EnableRendering = false;
		}

		const bool& GuardianWidget::IsEnableRendering() const
		{
			return this->EnableRendering;
		}

		void GuardianWidget::AddStyleToWidget(Style style)
		{
			this->WidgetStyleList.push_back(style);
		}

		void GuardianWidget::AddColorToWidget(Color color)
		{
			this->WidgetColorList.push_back(color);
		}

		void GuardianWidget::SetWidgetEventProcessFunction(const std::function<void()>& function)
		{
			this->WidgetEventProcessFunction = function;
		}

		const std::vector<GuardianWidget::Style>& GuardianWidget::GetWidgetStyleList() const
		{
			return this->WidgetStyleList;
		}

		const std::vector<GuardianWidget::Color>& GuardianWidget::GetWidgetColorList() const
		{
			return this->WidgetColorList;
		}

		void GuardianWidget::CallWidgetEventProcessFunction()
		{
			if (this->WidgetEventProcessFunction)
			{
				this->WidgetEventProcessFunction();
			}
		}
	}
}

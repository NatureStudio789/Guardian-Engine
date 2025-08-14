#include "GWidget.h"

namespace GE
{
	namespace EUI
	{
		void GWidget::EnableWidgetRendering()
		{
			this->EnableRendering = true;
		}

		void GWidget::DisableWidgetRendering()
		{
			this->EnableRendering = false;
		}

		const bool& GWidget::IsEnableRendering() const
		{
			return this->EnableRendering;
		}

		void GWidget::AddStyleToWidget(Style style)
		{
			this->WidgetStyleList.push_back(style);
		}

		void GWidget::AddColorToWidget(Color color)
		{
			this->WidgetColorList.push_back(color);
		}

		void GWidget::SetWidgetEventProcessFunction(const std::function<void()>& function)
		{
			this->WidgetEventProcessFunction = function;
		}

		const std::vector<GWidget::Style>& GWidget::GetWidgetStyleList() const
		{
			return this->WidgetStyleList;
		}

		const std::vector<GWidget::Color>& GWidget::GetWidgetColorList() const
		{
			return this->WidgetColorList;
		}

		const GUUID& GWidget::GetWidgetId() const noexcept
		{
			return this->WidgetId;
		}

		void GWidget::CallWidgetEventProcessFunction()
		{
			if (this->WidgetEventProcessFunction)
			{
				this->WidgetEventProcessFunction();
			}
		}
	}
}

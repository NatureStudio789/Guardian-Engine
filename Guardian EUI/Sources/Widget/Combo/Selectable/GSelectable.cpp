#include "GSelectable.h"

namespace GE
{
	namespace EUI
	{
		GSelectable::GSelectable(const std::string& label, const std::function<void()>& selectingCallbackFunction, bool selected)
		{
			this->SetSelectableLabel(label);
			this->SetSelectingCallbackFunction(selectingCallbackFunction);
			this->IsSelected = selected;
		}

		void GSelectable::SetSelectableLabel(const std::string& label)
		{
			this->SelectableLabel = label;
		}

		void GSelectable::SetSelectingCallbackFunction(const std::function<void()>& selectingCallbackFunction)
		{
			this->SelectingCallbackFunction = selectingCallbackFunction;
		}
		
		void GSelectable::Update()
		{

		}

		void GSelectable::Render()
		{
			bool selected = ImGui::Selectable(this->SelectableLabel.c_str(), this->IsSelected);

			this->CallWidgetEventProcessFunction();

			if (selected)
			{
				if (this->SelectingCallbackFunction)
				{
					this->SelectingCallbackFunction();
				}
			}
		}
	}
}

#include "GCombo.h"

namespace GE
{
	namespace EUI
	{
		GCombo::GCombo(const std::string& label, const std::string& showing)
		{
			this->SetComboLabel(label);
			this->SetComboShowing(showing);
		}

		void GCombo::SetComboLabel(const std::string& label)
		{
			this->ComboLabel = label;
		}

		void GCombo::SetComboShowing(const std::string& showing)
		{
			this->ComboShowing = showing;
		}

		void GCombo::AddWidgetToCombo(std::shared_ptr<GWidget> widget)
		{
			this->ComboWidgetList.push_back(widget);
		}

		void GCombo::ClearPanelWidget()
		{
			this->ComboWidgetList.clear();
		}

		void GCombo::Update()
		{
			for (auto& widget : this->ComboWidgetList)
			{
				widget->Update();
			}
		}

		void GCombo::Render()
		{
			if (ImGui::BeginCombo(this->ComboLabel.c_str(), this->ComboShowing.c_str()))
			{
				for (auto& widget : this->ComboWidgetList)
				{
					widget->Render();
				}

				ImGui::EndCombo();
			}
		}
	}
}

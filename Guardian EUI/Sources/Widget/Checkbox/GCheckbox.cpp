#include "GCheckbox.h"

namespace GE
{
	namespace  EUI
	{
		GCheckbox::GCheckbox(const std::string& label, bool* checked, std::function<void()> callback)
		{
			this->SetCheckboxLabel(label);
			this->SetCheckboxChecked(checked);
			this->SetCheckboxCallback(callback);
		}

		void GCheckbox::SetCheckboxLabel(const std::string& label)
		{
			this->CheckboxLabel = label;
		}

		void GCheckbox::SetCheckboxChecked(bool* checked)
		{
			this->Checked = checked;
		}

		void GCheckbox::SetCheckboxCallback(std::function<void()> callback)
		{
			this->CheckboxCallbackFunction = callback;
		}

		void GCheckbox::Update()
		{

		}

		void GCheckbox::Render()
		{
			bool checked = ImGui::Checkbox(this->CheckboxLabel.c_str(), this->Checked);

			this->CallWidgetEventProcessFunction();

			if (checked)
			{
				if (this->CheckboxCallbackFunction)
				{
					this->CheckboxCallbackFunction();
				}
			}
		}
	}
}

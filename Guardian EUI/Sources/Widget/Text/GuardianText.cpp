#include "GuardianText.h"

namespace GE
{
	namespace EUI
	{
		GuardianText::GuardianText(const std::string& label, const GVector4& color, bool isWrapped)
		{
			this->SetTextLabel(label);
		}

		void GuardianText::SetTextLabel(const std::string& label)
		{
			this->TextLabel = label;
		}

		void GuardianText::SetTextColor(const GVector4& color)
		{
			this->TextColor = color;
		}

		void GuardianText::SetTextWrapped(bool wrapped)
		{
			this->IsWrapped = wrapped;
		}

		void GuardianText::Update()
		{

		}

		void GuardianText::Render()
		{
			if (this->IsWrapped)
			{
				ImGui::TextWrapped(this->TextLabel.c_str());
			}
			else
			{
				ImGui::TextColored(
					ImVec4(this->TextColor.x, this->TextColor.y, this->TextColor.z, this->TextColor.w), this->TextLabel.c_str());
			}

			this->CallWidgetEventProcessFunction();
		}
	}
}

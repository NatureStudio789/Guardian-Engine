#include "GText.h"

namespace GE
{
	namespace EUI
	{
		GText::GText(const std::string& label, const GVector4& color, bool isWrapped)
		{
			this->SetTextLabel(label);
		}

		void GText::SetTextLabel(const std::string& label)
		{
			this->TextLabel = label;
		}

		void GText::SetTextColor(const GVector4& color)
		{
			this->TextColor = color;
		}

		void GText::SetTextWrapped(bool wrapped)
		{
			this->IsWrapped = wrapped;
		}

		void GText::Update()
		{

		}

		void GText::Render()
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

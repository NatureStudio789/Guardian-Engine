#include "GuardianImageButton.h"

namespace GE
{
	namespace EUI
	{
		GuardianImageButton::GuardianImageButton(const std::string& stringId,
			const std::function<void()>& callback, GuardianImage::Id imageId, const GVector2& buttonSize)
		{
			this->SetButtonId(stringId);
			this->SetButtonCallbackFunction(callback);
			this->SetButtonImageId(imageId);
			this->SetButtonSize(buttonSize);
		}

		void GuardianImageButton::SetButtonCallbackFunction(const std::function<void()>& callback)
		{
			this->ButtonCallbackFunction = callback;
		}

		void GuardianImageButton::SetButtonId(const std::string& id)
		{
			this->ButtonId = id;
		}

		void GuardianImageButton::SetButtonImageId(GuardianImage::Id imageId)
		{
			this->ButtonImageId = imageId;
		}

		void GuardianImageButton::SetButtonSize(const GVector2& size)
		{
			this->ButtonSize = size;
		}

		void GuardianImageButton::Update()
		{

		}

		void GuardianImageButton::Render()
		{
			bool isClicked = ImGui::ImageButton(this->ButtonId.c_str(),
				this->ButtonImageId, ImVec2(this->ButtonSize.x, this->ButtonSize.y));

			this->CallWidgetEventProcessFunction();

			if (isClicked)
			{
				if (this->ButtonCallbackFunction)
				{
					this->ButtonCallbackFunction();
				}
			}
		}
	}
}

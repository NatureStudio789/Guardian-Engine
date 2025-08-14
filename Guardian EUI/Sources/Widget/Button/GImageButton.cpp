#include "GImageButton.h"

namespace GE
{
	namespace EUI
	{
		GImageButton::GImageButton(const std::string& stringId,
			const std::function<void()>& callback, GImage::Id imageId, const GVector2& buttonSize)
		{
			this->SetButtonId(stringId);
			this->SetButtonCallbackFunction(callback);
			this->SetButtonImageId(imageId);
			this->SetButtonSize(buttonSize);
		}

		void GImageButton::SetButtonCallbackFunction(const std::function<void()>& callback)
		{
			this->ButtonCallbackFunction = callback;
		}

		void GImageButton::SetButtonId(const std::string& id)
		{
			this->ButtonId = id;
		}

		void GImageButton::SetButtonImageId(GImage::Id imageId)
		{
			this->ButtonImageId = imageId;
		}

		void GImageButton::SetButtonSize(const GVector2& size)
		{
			this->ButtonSize = size;
		}

		void GImageButton::Update()
		{

		}

		void GImageButton::Render()
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

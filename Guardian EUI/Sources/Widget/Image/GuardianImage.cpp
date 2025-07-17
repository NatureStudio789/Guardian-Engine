#include "GuardianImage.h"

namespace GE
{
	namespace EUI
	{
		GuardianImage::GuardianImage(Id imageId,
			const GVector2& size, const GVector4& borderColor)
		{
			this->SetImageId(imageId);
			this->SetImageSize(size);
			this->SetImageBorderColor(borderColor);
		}

		void GuardianImage::SetImageId(Id imageId)
		{
			this->ImageId = imageId;
		}

		void GuardianImage::SetImageSize(const GVector2& size)
		{
			this->ImageSize = size;
		}

		void GuardianImage::SetImageBorderColor(const GVector4& borderColor)
		{
			this->ImageBorderColor = borderColor;
		}

		void GuardianImage::Update()
		{

		}

		void GuardianImage::Render()
		{
			ImGui::Image(this->ImageId, ImVec2(this->ImageSize.x, this->ImageSize.y),
				{ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
				ImVec4(this->ImageBorderColor.x, this->ImageBorderColor.y, this->ImageBorderColor.z, this->ImageBorderColor.w));
		}
	}
}

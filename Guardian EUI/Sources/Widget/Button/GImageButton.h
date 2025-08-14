#ifndef _GE_GIMAGEBUTTON_H_
#define _GE_GIMAGEBUTTON_H_
#include "../Image/GImage.h"

namespace GE
{
	namespace EUI
	{
		class GImageButton : public GWidget
		{
		public:
			GImageButton() = default;
			GImageButton(const std::string& stringId, const std::function<void()>& callback,
				GImage::Id imageId, const GVector2& buttonSize);
			GImageButton(const GImageButton&) = default;

			void SetButtonCallbackFunction(const std::function<void()>& callback);
			void SetButtonId(const std::string& id);
			void SetButtonImageId(GImage::Id imageId);
			void SetButtonSize(const GVector2& size);

			void Update() override;
			void Render() override;

		private:
			std::string ButtonId;
			GImage::Id ButtonImageId;
			GVector2 ButtonSize;

			std::function<void()> ButtonCallbackFunction;
		};
	}
}

#endif
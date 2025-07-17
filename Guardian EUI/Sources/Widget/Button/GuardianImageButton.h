#ifndef _GE_GUARDIANIMAGEBUTTON_H_
#define _GE_GUARDIANIMAGEBUTTON_H_
#include "../Image/GuardianImage.h"

namespace GE
{
	namespace EUI
	{
		class GuardianImageButton : public GuardianWidget
		{
		public:
			GuardianImageButton() = default;
			GuardianImageButton(const std::string& stringId, const std::function<void()>& callback,
				GuardianImage::Id imageId, const GVector2& buttonSize);
			GuardianImageButton(const GuardianImageButton&) = default;

			void SetButtonCallbackFunction(const std::function<void()>& callback);
			void SetButtonId(const std::string& id);
			void SetButtonImageId(GuardianImage::Id imageId);
			void SetButtonSize(const GVector2& size);

			void Update() override;
			void Render() override;

		private:
			std::string ButtonId;
			GuardianImage::Id ButtonImageId;
			GVector2 ButtonSize;

			std::function<void()> ButtonCallbackFunction;
		};
	}
}

#endif
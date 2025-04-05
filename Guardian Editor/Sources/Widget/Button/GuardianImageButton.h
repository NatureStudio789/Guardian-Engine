#ifndef _GE_GUARDIANIMAGEBUTTON_H_
#define _GE_GUARDIANIMAGEBUTTON_H_
#include "../GuardianWidget.h"

namespace GE
{
	class GuardianImageButton : public GuardianWidget
	{
	public:
		using ImageId = ImTextureID;

	public:
		GuardianImageButton() = default;
		GuardianImageButton(const GString& stringId, const std::function<void()>& callback,
			ImageId imageId, const GVector2& buttonSize);
		GuardianImageButton(const GuardianImageButton&) = default;

		void SetButtonCallbackFunction(const std::function<void()>& callback);
		void SetButtonId(const GString& id);
		void SetButtonImageId(ImageId imageId);
		void SetButtonSize(const GVector2& size);

		void Update() override;
		void Render() override;

	private:
		GString ButtonId;
		ImageId ButtonImageId;
		GVector2 ButtonSize;

		std::function<void()> ButtonCallbackFunction;
	};
}

#endif
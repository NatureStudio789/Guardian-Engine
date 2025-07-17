#ifndef _GE_GUARDIANBUTTON_H_
#define _GE_GUARDIANBUTTON_H_
#include "../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianButton : public GuardianWidget
		{
		public:
			GuardianButton() = default;
			GuardianButton(const std::string& label,
				const std::function<void()>& callback, const GVector2& size);
			GuardianButton(const GuardianButton&) = default;

			void SetButtonLabel(const std::string& label);
			void SetButtonCallbackFunction(const std::function<void()>& callback);
			void SetButtonSize(const GVector2& size);

			void Update() override;
			void Render() override;

		private:
			std::string ButtonLabel;
			GVector2 ButtonSize;
			std::function<void()> ButtonCallbackFunction;
		};
	}
}

#endif
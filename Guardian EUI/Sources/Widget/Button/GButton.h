#ifndef _GE_GBUTTON_H_
#define _GE_GBUTTON_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GButton : public GWidget
		{
		public:
			GButton() = default;
			GButton(const std::string& label,
				const std::function<void()>& callback, const GVector2& size);
			GButton(const GButton&) = default;

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
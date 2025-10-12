#ifndef _GE_GSELECTABLE_H_
#define _GE_GSELECTABLE_H_
#include "../../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GSelectable : public GWidget
		{
		public:
			GSelectable() = default;
			GSelectable(const std::string& label, const std::function<void()>& selectingCallbackFunction, bool selected = false);
			GSelectable(const GSelectable&) = default;

			void SetSelectableLabel(const std::string& label);
			void SetSelectingCallbackFunction(const std::function<void()>& selectingCallbackFunction);

			void Update() override;
			void Render() override;

		private:
			std::string SelectableLabel;
			std::function<void()> SelectingCallbackFunction;
			bool IsSelected;
		};
	}
}

#endif	
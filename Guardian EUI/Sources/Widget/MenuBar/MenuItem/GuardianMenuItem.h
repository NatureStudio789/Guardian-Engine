#ifndef _GE_GUARDIANMENUITEM_H_
#define _GE_GUARDIANMENUITEM_H_
#include "../../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianMenuItem : public GuardianWidget
		{
		public:
			GuardianMenuItem() = default;
			GuardianMenuItem(const std::string& label,
				const std::function<void()>& callback);
			GuardianMenuItem(const GuardianMenuItem&) = default;
			~GuardianMenuItem() = default;

			void SetMenuItemLabel(const std::string& label);
			void SetMenuItemCallback(const std::function<void()> callback);

			void Update() override;
			void Render() override;

		private:
			std::string ItemLabel;
			std::function<void()> ItemCallback;
		};
	}
}

#endif
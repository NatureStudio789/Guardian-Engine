#ifndef _GE_GUARDIANMENUITEM_H_
#define _GE_GUARDIANMENUITEM_H_
#include "../../GuardianWidget.h"

namespace GE
{
	class GuardianMenuItem : public GuardianWidget
	{
	public:
		GuardianMenuItem() = default;
		GuardianMenuItem(const GString& label, 
			const std::function<void()>& callback);
		GuardianMenuItem(const GuardianMenuItem&) = default;
		~GuardianMenuItem() = default;

		void SetMenuItemLabel(const GString& label);
		void SetMenuItemCallback(const std::function<void()> callback);

		void Update() override;
		void Render() override;

	private:
		GString ItemLabel;
		std::function<void()> ItemCallback;
	};
}

#endif
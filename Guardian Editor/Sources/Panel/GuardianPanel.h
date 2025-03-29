#ifndef _GE_GUARDIANPANEL_H_
#define _GE_GUARDIANPANEL_H_
#include "../Widget/GuardianWidget.h"

namespace GE
{
	class GuardianPanel
	{
	public:
		GuardianPanel() = default;
		GuardianPanel(const GuardianPanel& ) = default;
		virtual ~GuardianPanel() = default;

		virtual void Update() {}

		virtual void Render() {}

		const GString& GetPanelName() const noexcept;

	protected:
		GString PanelName;
	};
}

#endif
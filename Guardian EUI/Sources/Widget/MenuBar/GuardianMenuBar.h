#ifndef _GE_GUARDIANMENUBAR_H_
#define _GE_GUARDIANMENUBAR_H_
#include "../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianMenuBar : public GuardianWidget
		{
		public:
			GuardianMenuBar() = default;
			GuardianMenuBar(const GuardianMenuBar&) = default;
			virtual ~GuardianMenuBar() = default;

			void AddWidgetToMenuBar(std::shared_ptr<GuardianWidget> widget);
			void ClearMenuBarWidgets();

			virtual void Update() override;
			virtual void Render() override;

		protected:
			void RenderMenuBarWidgets();

			std::vector<std::shared_ptr<GuardianWidget>> MenuBarWidgetList;
		};
	}
}

#endif
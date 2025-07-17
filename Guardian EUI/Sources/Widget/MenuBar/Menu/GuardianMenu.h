#ifndef _GE_GUARDIANMENU_H_
#define _GE_GUARDIANMENU_H_
#include"../../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianMenu : public GuardianWidget
		{
		public:
			GuardianMenu() = default;
			GuardianMenu(const GuardianMenu&) = default;
			virtual ~GuardianMenu() = default;

			void SetMenuLabel(const std::string& label);
			void AddWidgetToMenu(std::shared_ptr<GuardianWidget> widget);
			void ClearMenuWidgets();

			virtual void Update() override;
			virtual void Render() override;

		private:
			void RenderMenuWidgets();

			std::string MenuLabel;
			std::vector<std::shared_ptr<GuardianWidget>> MenuWidgetList;
		};
	}
}

#endif
#ifndef _GE_GMENU_H_
#define _GE_GMENU_H_
#include"../../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GMenu : public GWidget
		{
		public:
			GMenu() = default;
			GMenu(const GMenu&) = default;
			virtual ~GMenu() = default;

			void SetMenuLabel(const std::string& label);
			void AddWidgetToMenu(std::shared_ptr<GWidget> widget);
			void ClearMenuWidgets();

			virtual void Update() override;
			virtual void Render() override;

		private:
			void RenderMenuWidgets();

			std::string MenuLabel;
			std::vector<std::shared_ptr<GWidget>> MenuWidgetList;
		};
	}
}

#endif
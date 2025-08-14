#ifndef _GE_GMENUBAR_H_
#define _GE_GMENUBAR_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GMenuBar : public GWidget
		{
		public:
			GMenuBar() = default;
			GMenuBar(const GMenuBar&) = default;
			virtual ~GMenuBar() = default;

			void AddWidgetToMenuBar(std::shared_ptr<GWidget> widget);
			void ClearMenuBarWidgets();

			virtual void Update() override;
			virtual void Render() override;

		protected:
			void RenderMenuBarWidgets();

			std::vector<std::shared_ptr<GWidget>> MenuBarWidgetList;
		};
	}
}

#endif
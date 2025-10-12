#ifndef _GE_GCOMBO_H_
#define _GE_GCOMBO_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GCombo : public GWidget
		{
		public:
			GCombo() = default;
			GCombo(const std::string& label, const std::string& showing);
			GCombo(const GCombo& ) = default;

			void SetComboLabel(const std::string& label);
			void SetComboShowing(const std::string& showing);

			void AddWidgetToCombo(std::shared_ptr<GWidget> widget);
			void ClearPanelWidget();

			void Update() override;
			void Render() override;

		private:
			std::string ComboLabel;
			std::string ComboShowing;

			std::vector<std::shared_ptr<GWidget>> ComboWidgetList;
		};
	}
}

#endif
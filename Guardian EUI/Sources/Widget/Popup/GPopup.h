#ifndef _GE_GPOPUP_H_
#define _GE_GPOPUP_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GPopup : public GWidget
		{
		public:
			GPopup() = default;
			GPopup(const std::string& name);
			GPopup(const GPopup&) = default;

			void AddWidgetToPopup(std::shared_ptr<GWidget> widget);
			void ClearPopupWidget();

			//This function can only be called in the editor render frame.
			void ActivePopup();

			void Update() override;
			void Render() override;
			
		private:
			std::string PopupName;

			std::vector<std::shared_ptr<GWidget>> WidgetList;
		};
	}
}

#endif
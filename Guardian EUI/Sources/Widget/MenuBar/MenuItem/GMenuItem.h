#ifndef _GE_GMENUITEM_H_
#define _GE_GMENUITEM_H_
#include "../../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GMenuItem : public GWidget
		{
		public:
			GMenuItem() = default;
			GMenuItem(const std::string& label,
				const std::function<void()>& callback);
			GMenuItem(const GMenuItem&) = default;
			~GMenuItem() = default;

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
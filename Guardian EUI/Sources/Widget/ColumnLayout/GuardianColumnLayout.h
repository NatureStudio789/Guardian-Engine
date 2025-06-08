#ifndef _GE_GUARDIANCOLUMNLAYOUT_H_
#define _GE_GUARDIANCOLUMNLAYOUT_H_
#include "../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianColumnLayout : public GuardianWidget
		{
		public:
			GuardianColumnLayout() = default;
			GuardianColumnLayout(const GuardianColumnLayout&) = default;
			virtual ~GuardianColumnLayout() = default;

			void AddWidgetToLayout(std::shared_ptr<GuardianWidget> widget);
			void SetColumnCount(int count);
			void ClearLayoutWidgets();

			virtual void Update() override;
			virtual void Render() override;

			const int& GetColumnCount() const noexcept;

		protected:
			void RenderLayoutWidgets();

			std::vector<std::shared_ptr<GuardianWidget>> LayoutWidgetList;
			int LayoutColumnCount;
		};
	}
}

#endif
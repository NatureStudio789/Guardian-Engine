#ifndef _GE_GCOLUMNLAYOUT_H_
#define _GE_GCOLUMNLAYOUT_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GColumnLayout : public GWidget
		{
		public:
			GColumnLayout() = default;
			GColumnLayout(const GColumnLayout&) = default;
			virtual ~GColumnLayout() = default;

			void AddWidgetToLayout(std::shared_ptr<GWidget> widget);
			void SetColumnCount(int count);
			void ClearLayoutWidgets();

			virtual void Update() override;
			virtual void Render() override;

			const int& GetColumnCount() const noexcept;

		protected:
			void RenderLayoutWidgets();

			std::vector<std::shared_ptr<GWidget>> LayoutWidgetList;
			int LayoutColumnCount;
		};
	}
}

#endif
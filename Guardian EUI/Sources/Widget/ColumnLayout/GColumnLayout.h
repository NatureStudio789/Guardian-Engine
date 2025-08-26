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

			void AddColumn(std::vector<std::shared_ptr<GWidget>> column);
			void SetColumnCount(int count);
			void ClearLayoutWidgets();

			virtual void Update() override;
			virtual void Render() override;

			const int& GetColumnCount() const noexcept;

		protected:
			void RenderLayoutWidgets();

			std::vector<std::vector<std::shared_ptr<GWidget>>> ColumnList;
			int LayoutColumnCount;
		};
	}
}

#endif
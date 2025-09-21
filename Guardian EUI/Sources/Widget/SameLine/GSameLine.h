#ifndef _GE_GSAMELINE_H_
#define _GE_GSAMELINE_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GSameLine : public GWidget
		{
		public:
			using GWidget::GWidget;

			void Update() override;
			void Render() override;
		};
	}
}

#endif
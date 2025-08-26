#ifndef _GE_GSEPARATOR_H_
#define _GE_GSEPARATOR_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GSeparator : public GWidget
		{
		public:
			GSeparator() = default;
			GSeparator(const GSeparator&) = default;

			void Update() override;
			void Render() override;

		private:
		};
	}
}

#endif
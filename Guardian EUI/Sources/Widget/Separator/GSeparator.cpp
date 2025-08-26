#include "GSeparator.h"

namespace GE
{
	namespace EUI
	{
		void GSeparator::Update()
		{

		}

		void GSeparator::Render()
		{
			ImGui::Separator();

			this->CallWidgetEventProcessFunction();
		}
	}
}

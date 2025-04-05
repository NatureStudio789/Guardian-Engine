#ifndef _GE_GUARDIANRENDEREREDITOR_H_
#define _GE_GUARDIANRENDEREREDITOR_H_
#include "GuardianEditor.h"

namespace GE
{
	class GuardianRendererEditor : public GuardianEditor
	{
	public:
		GuardianRendererEditor() = default;
		GuardianRendererEditor(const GuardianRendererEditor&) = default;

		void Initialize() override;

		void Update() override;

	private:
		std::shared_ptr<GuardianPanel> MainPanel;
		std::shared_ptr<GuardianButton> TestButton;
		std::shared_ptr<GuardianText> FPSText;
		std::shared_ptr<GuardianText> EventText;
	};
}

#endif
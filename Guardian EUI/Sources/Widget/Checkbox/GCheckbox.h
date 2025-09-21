#ifndef _GE_GCHECKBOX_H_
#define _GE_GCHECKBOX_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GCheckbox : public GWidget
		{
		public:
			GCheckbox() = default;
			GCheckbox(const std::string& label, bool* checked, std::function<void()> callback);
			GCheckbox(const GCheckbox&) = default;

			void SetCheckboxLabel(const std::string& label);
			void SetCheckboxChecked(bool* checked);
			void SetCheckboxCallback(std::function<void()> callback);

			void Update() override;
			void Render() override;

		private:
			std::string CheckboxLabel;
			bool* Checked;
			std::function<void()> CheckboxCallbackFunction;
		};
	}
}

#endif
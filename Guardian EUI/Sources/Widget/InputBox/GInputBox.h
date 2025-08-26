#ifndef _GE_GINPUTBOX_H_
#define _GE_GINPUTBOX_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GInputBox : public GWidget
		{
		public:
			GInputBox() = default;
			GInputBox(const std::string& label, std::any inputData);
			GInputBox(const GInputBox&) = default;

			void SetInputBoxLabel(const std::string& label);
			void SetInputData(std::any data);

			void Update() override;
			void Render() override;

		private:
			std::string InputBoxLabel;
			std::any InputData;
		};
	}
}

#endif
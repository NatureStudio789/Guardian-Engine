#ifndef _GE_GTEXT_H_
#define _GE_GTEXT_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GText : public GWidget
		{
		public:
			GText() = default;
			GText(const std::string& label,
				const GVector4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, bool isWrapped = false);
			GText(const GText&) = default;

			void SetTextLabel(const std::string& label);
			void SetTextColor(const GVector4& color);
			void SetTextWrapped(bool wrapped);

			void Update() override;
			void Render() override;

		private:
			std::string TextLabel;
			GVector4 TextColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			bool IsWrapped = false;
		};
	}
}

#endif
#ifndef _GE_GUARDIANTEXT_H_
#define _GE_GUARDIANTEXT_H_
#include "../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianText : public GuardianWidget
		{
		public:
			GuardianText() = default;
			GuardianText(const GString& label,
				const GVector4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, bool isWrapped = false);
			GuardianText(const GuardianText&) = default;

			void SetTextLabel(const GString& label);
			void SetTextColor(const GVector4& color);
			void SetTextWrapped(bool wrapped);

			void Update() override;
			void Render() override;

		private:
			GString TextLabel;
			GVector4 TextColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			bool IsWrapped = false;
		};
	}
}

#endif
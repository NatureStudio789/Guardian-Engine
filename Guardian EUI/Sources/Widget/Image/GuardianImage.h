#ifndef _GE_GUARDIANIMAGE_H_
#define _GE_GUARDIANIMAGE_H_
#include "../GuardianWidget.h"

namespace GE
{
	namespace EUI
	{
		class GuardianImage : public GuardianWidget
		{
		public:
			using Id = unsigned long long;

		public:
			GuardianImage() = default;
			GuardianImage(Id imageId,
				const GVector2& size, const GVector4& borderColor = { 0.0f, 0.0f, 0.0f, 0.0f });
			GuardianImage(const GuardianImage&) = default;

			void SetImageId(Id imageId);
			void SetImageSize(const GVector2& size);
			void SetImageBorderColor(const GVector4& borderColor);

			void Update() override;
			void Render() override;

		private:
			Id ImageId;
			GVector2 ImageSize;
			GVector4 ImageBorderColor;
		};
	}
}

#endif	
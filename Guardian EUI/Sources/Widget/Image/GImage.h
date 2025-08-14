#ifndef _GE_GIMAGE_H_
#define _GE_GIMAGE_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GImage : public GWidget
		{
		public:
			using Id = unsigned long long;

		public:
			GImage() = default;
			GImage(Id imageId,
				const GVector2& size, const GVector4& borderColor = { 0.0f, 0.0f, 0.0f, 0.0f });
			GImage(const GImage&) = default;

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
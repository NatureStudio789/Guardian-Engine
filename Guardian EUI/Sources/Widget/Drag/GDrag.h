#ifndef _GE_GDRAG_H_
#define _GE_GDRAG_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GDrag : public GWidget
		{
		public:
			GDrag() = default;
			GDrag(const std::string& label,
				std::any dragValue, float dragingSpeed = 0.1f, float min = 0.0f, float max = 0.0f);
			GDrag(const GDrag&) = default;

			void SetDragLabel(const std::string& label);
			void SetDragValue(std::any dragValue);
			void SetDragRange(float min, float max);
			void SetDragSpeed(float speed);

			void Update() override;
			void Render() override;

		private:
			std::string DragLabel;
			std::any DragValue;
			float DragingSpeed;
			float MaxValue;
			float MinValue;
		};
	}
}

#endif
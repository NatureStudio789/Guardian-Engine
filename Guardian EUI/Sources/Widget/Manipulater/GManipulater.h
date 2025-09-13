#ifndef _GE_GMANIPULATER_H_
#define _GE_GMANIPULATER_H_
#include "../GWidget.h"

namespace GE
{
	namespace EUI
	{
		class GManipulater : public GWidget
		{
		public:
			enum Operation
			{
				GE_OPERATION_TRANSLATE_X = ImGuizmo::TRANSLATE_X,
				GE_OPERATION_TRANSLATE_Y = ImGuizmo::TRANSLATE_Y,
				GE_OPERATION_TRANSLATE_Z = ImGuizmo::TRANSLATE_Z,
				GE_OPERATION_ROTATE_X = ImGuizmo::ROTATE_X,
				GE_OPERATION_ROTATE_Y = ImGuizmo::ROTATE_Y,
				GE_OPERATION_ROTATE_Z = ImGuizmo::ROTATE_Z,
				GE_OPERATION_ROTATE_SCREEN = ImGuizmo::ROTATE_SCREEN,
				GE_OPERATION_SCALE_X = ImGuizmo::SCALE_X,
				GE_OPERATION_SCALE_Y = ImGuizmo::SCALE_Y,
				GE_OPERATION_SCALE_Z = ImGuizmo::SCALE_Z,
				GE_OPERATION_BOUNDS = ImGuizmo::BOUNDS,
				GE_OPERATION_SCALE_XU = ImGuizmo::SCALE_XU,
				GE_OPERATION_SCALE_YU = ImGuizmo::SCALE_YU,
				GE_OPERATION_SCALE_ZU = ImGuizmo::SCALE_ZU,

				GE_OPERATION_TRANSLATE = ImGuizmo::TRANSLATE,
				GE_OPERATION_ROTATE = ImGuizmo::ROTATE,
				GE_OPERATION_SCALE = ImGuizmo::SCALE,
				GE_OPERATION_SCALEU = ImGuizmo::SCALEU,
				GE_OPERATION_UNIVERSAL = ImGuizmo::UNIVERSAL
			};

		public:
			GManipulater() = default;
			GManipulater(const float* view, const float* projection, float* transform,
				GVector2 rectPosition, GVector2 rectSize, const std::function<void(const Operation&)>& operationProcessFunction);
			GManipulater(const GManipulater&) = default;

			void SetViewMatrix(const float* view);
			void SetProjectionMatrix(const float* projection);
			void SetTransformMatrix(float* transform);
			void SetManipulateRect(GVector2 rectPosition, GVector2 rectSize);
			void SetCurrentOperation(Operation operation);
			void SetOperationProcessFunction(const std::function<void(const Operation&)>& operationProcessFunction);

			void Update() override;
			void Render() override;

		private:
			GVector2 RectPosition;
			GVector2 RectSize;
			const float* ViewMatrix;
			const float* ProjectionMatrix;
			float* TransformMatrix;

			Operation CurrentOperation;
			std::function<void(const Operation& )> OperationProcessFunction;
		};
	}
}

#endif
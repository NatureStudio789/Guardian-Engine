#include "GManipulater.h"

namespace GE
{
	namespace EUI
	{
		GManipulater::GManipulater(const float* view, const float* projection, 
			float* transform, GVector2 rectPosition, GVector2 rectSize, const std::function<void(const Operation&)>& operationProcessFunction)
		{
			this->SetViewMatrix(view);
			this->SetProjectionMatrix(projection);
			this->SetTransformMatrix(transform);
			
			this->SetCurrentOperation(GE_OPERATION_TRANSLATE);

			this->SetManipulateRect(rectPosition, rectSize);

			this->SetOperationProcessFunction(operationProcessFunction);
		}

		void GManipulater::SetViewMatrix(const float* view)
		{
			this->ViewMatrix = view;
		}

		void GManipulater::SetProjectionMatrix(const float* projection)
		{
			this->ProjectionMatrix = projection;
		}

		void GManipulater::SetTransformMatrix(float* transform)
		{
			this->TransformMatrix = transform;
		}

		void GManipulater::SetManipulateRect(GVector2 rectPosition, GVector2 rectSize)
		{
			this->RectPosition = rectPosition;
			this->RectSize = rectSize;
		}

		void GManipulater::SetCurrentOperation(Operation operation)
		{
			this->CurrentOperation = operation;
		}

		void GManipulater::SetOperationProcessFunction(const std::function<void(const Operation&)>& operationProcessFunction)
		{
			this->OperationProcessFunction = operationProcessFunction;
		}

		void GManipulater::Update()
		{

		}

		void GManipulater::Render()
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(this->RectPosition.x, this->RectPosition.y, this->RectSize.x, this->RectSize.y);

			ImGuizmo::Manipulate(this->ViewMatrix, this->ProjectionMatrix,
				(ImGuizmo::OPERATION)this->CurrentOperation, ImGuizmo::LOCAL, this->TransformMatrix);

			if (this->OperationProcessFunction)
			{
				this->OperationProcessFunction(this->CurrentOperation);
			}
		}
	}
}

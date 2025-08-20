#include "GDrag.h"

namespace GE
{
	namespace EUI
	{
		GDrag::GDrag(const std::string& label, std::any dragValue, float dragingSpeed, float min, float max)
		{
			this->SetDragLabel(label);
			this->SetDragSpeed(dragingSpeed);
			this->SetDragRange(min, max);
			this->SetDragValue(dragValue);
		}

		void GDrag::SetDragLabel(const std::string& label)
		{
			this->DragLabel = label;
		}

		void GDrag::SetDragValue(std::any dragValue)
		{
			this->DragValue = dragValue;
		}

		void GDrag::SetDragRange(float min, float max)
		{
			this->MinValue = min;
			this->MaxValue = max;
		}

		void GDrag::SetDragSpeed(float speed)
		{
			this->DragingSpeed = speed;
		}

		void GDrag::Update()
		{

		}

		void GDrag::Render()
		{
			if (this->DragValue.type() == typeid(float*))
			{
				ImGui::DragFloat(this->DragLabel.c_str(), std::any_cast<float*>(this->DragValue), this->DragingSpeed,
					this->MinValue, this->MaxValue);;
			}
			else if (this->DragValue.type() == typeid(GVector2*))
			{
				GVector2* value = std::any_cast<GVector2*>(this->DragValue);
				float float2[2] = { value->x, value->y };
				if (ImGui::DragFloat2(this->DragLabel.c_str(), float2, this->DragingSpeed, this->MinValue, this->MaxValue))
				{
					value->x = float2[0];
					value->y = float2[1];
				}
			}
			else if (this->DragValue.type() == typeid(GVector3*))
			{
				GVector3* value = std::any_cast<GVector3*>(this->DragValue);
				float float3[3] = { value->x, value->y, value->z };
				if (ImGui::DragFloat3(this->DragLabel.c_str(), float3, this->DragingSpeed, this->MinValue, this->MaxValue))
				{
					value->x = float3[0];
					value->y = float3[1];
					value->z = float3[2];
				}
			}
			else if (this->DragValue.type() == typeid(GVector4*))
			{
				GVector4* value = std::any_cast<GVector4*>(this->DragValue);
				float float4[4] = { value->x, value->y, value->z, value->w };
				if (ImGui::DragFloat4(this->DragLabel.c_str(), float4, this->DragingSpeed, this->MinValue, this->MaxValue))
				{
					value->x = float4[0];
					value->y = float4[1];
					value->z = float4[2];
					value->w = float4[3];
				}
			}
			else
			{
				throw GUARDIAN_ERROR_EXCEPTION("Unsupported value type for drag!");
			}
		}
	}
}

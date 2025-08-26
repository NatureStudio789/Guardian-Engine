#include "GInputBox.h"

namespace GE
{
	namespace EUI
	{
		GInputBox::GInputBox(const std::string& label, std::any inputData)
		{
			this->SetInputBoxLabel(label);
			this->SetInputData(inputData);
		}

		void GInputBox::SetInputBoxLabel(const std::string& label)
		{
			this->InputBoxLabel = label;
		}

		void GInputBox::SetInputData(std::any data)
		{
			this->InputData = data;
		}

		void GInputBox::Update()
		{

		}

		void GInputBox::Render()
		{
			if (this->InputData.type() == typeid(float*))
			{
				ImGui::InputFloat(this->InputBoxLabel.c_str(), std::any_cast<float*>(this->InputData));

				this->CallWidgetEventProcessFunction();
			}
			else if (this->InputData.type() == typeid(GVector2*))
			{
				GVector2* value = std::any_cast<GVector2*>(this->InputData);
				float float2[2] = { value->x, value->y };
				bool isModified = ImGui::InputFloat2(this->InputBoxLabel.c_str(), float2);

				this->CallWidgetEventProcessFunction();

				if (isModified)
				{
					value->x = float2[0];
					value->y = float2[1];
				}
			}
			else if (this->InputData.type() == typeid(GVector3*))
			{
				GVector3* value = std::any_cast<GVector3*>(this->InputData);
				float float3[3] = { value->x, value->y, value->z };
				bool isModified = ImGui::InputFloat3(this->InputBoxLabel.c_str(), float3);

				this->CallWidgetEventProcessFunction();

				if (isModified)
				{
					value->x = float3[0];
					value->y = float3[1];
					value->z = float3[2];
				}
			}
			else if (this->InputData.type() == typeid(GVector4*))
			{
				GVector4* value = std::any_cast<GVector4*>(this->InputData);
				float float4[4] = { value->x, value->y, value->z, value->w };
				bool isModified = ImGui::InputFloat4(this->InputBoxLabel.c_str(), float4);

				this->CallWidgetEventProcessFunction();

				if (isModified)
				{
					value->x = float4[0];
					value->y = float4[1];
					value->z = float4[2];
					value->w = float4[3];
				}
			}
			else if (this->InputData.type() == typeid(std::string*))
			{
				std::string value = *std::any_cast<std::string*>(this->InputData);
				char* buffer = value.data();
				bool isModified = ImGui::InputText(this->InputBoxLabel.c_str(), buffer, 256);

				this->CallWidgetEventProcessFunction();

				if (isModified)
				{
					*std::any_cast<std::string*>(this->InputData) = buffer;
				}
			}
		}
	}
}

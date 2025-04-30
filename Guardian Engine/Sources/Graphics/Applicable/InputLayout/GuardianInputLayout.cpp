#include "GuardianInputLayout.h"

namespace GE
{
	GuardianInputLayout::GuardianInputLayout(const GuardianInputLayout& other)
	{
		this->InputLayoutObject = other.InputLayoutObject;
	}

	GuardianInputLayout::GuardianInputLayout(
		std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize)
	{
		this->InitializeInputLayout(vertexShader, data, dataSize);
	}

	GuardianInputLayout::~GuardianInputLayout()
	{
	}

	void GuardianInputLayout::InitializeInputLayout(
		std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize)
	{
		HRESULT hr = GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDevice()->CreateInputLayout(data, dataSize,
			vertexShader->GetVertexShaderBuffer()->GetBufferPointer(), vertexShader->GetVertexShaderBuffer()->GetBufferSize(),
			this->InputLayoutObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianInputLayout::Apply()
	{
		GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->IASetInputLayout(this->InputLayoutObject.Get());
	}

	WRL::ComPtr<ID3D11InputLayout> GuardianInputLayout::GetInputLayoutObject()
	{
		return this->InputLayoutObject;
	}

	std::shared_ptr<GuardianInputLayout> GuardianInputLayout::CreateNewInputLayout(
		std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize)
	{
		return std::make_shared<GuardianInputLayout>(vertexShader, data, dataSize);
	}
}

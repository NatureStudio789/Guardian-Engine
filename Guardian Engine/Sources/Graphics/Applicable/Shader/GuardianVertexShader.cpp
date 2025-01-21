#include "GuardianVertexShader.h"

namespace guardian
{
	GuardianVertexShader::GuardianVertexShader(const GuardianVertexShader& other)
	{
		this->VertexShaderBuffer = other.VertexShaderBuffer;
		this->VertexShaderObject = other.VertexShaderObject;
	}

	GuardianVertexShader::GuardianVertexShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
	{
		this->InitializeVertexShader(graphics, shaderFilePath);
	}

	GuardianVertexShader::~GuardianVertexShader()
	{
	}

	void GuardianVertexShader::InitializeVertexShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
	{
		std::ifstream ShaderFile(shaderFilePath.c_str());
		std::stringstream FileBuffer;
		if (!ShaderFile.is_open())
		{
			throw GUARDIAN_FAILURE_EXCEPTION("std::stringstream::stringstream()");
		}

		FileBuffer << ShaderFile.rdbuf();
		GString ShaderCode(FileBuffer.str());

		GWideString WideShaderFilePath = GuardianConverter::StringToWideString(shaderFilePath);
		HRESULT hr = D3DCompile(ShaderCode.c_str(), ShaderCode.size(), null, null, null, "main", "vs_5_0", 0, 0,
			this->VertexShaderBuffer.GetAddressOf(), null);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		hr = graphics->GetGraphicsDevice()->CreateVertexShader(this->VertexShaderBuffer->GetBufferPointer(),
			this->VertexShaderBuffer->GetBufferSize(), null, this->VertexShaderObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianVertexShader::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->VSSetShader(this->VertexShaderObject.Get(), null, 0);
	}

	WRL::ComPtr<ID3D11VertexShader> GuardianVertexShader::GetVertexShaderObject() noexcept
	{
		return this->VertexShaderObject;
	}

	WRL::ComPtr<ID3D10Blob> GuardianVertexShader::GetVertexShaderBuffer() noexcept
	{
		return this->VertexShaderBuffer;
	}

	std::shared_ptr<GuardianVertexShader> GuardianVertexShader::CreateNewVertexShader(
		std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
	{
		return std::make_shared<GuardianVertexShader>(graphics, shaderFilePath);
	}
}

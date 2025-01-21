#include "GuardianPixelShader.h"

namespace guardian
{
	GuardianPixelShader::GuardianPixelShader(const GuardianPixelShader& other)
	{
		this->PixelShaderBuffer = other.PixelShaderBuffer;
		this->PixelShaderObject = other.PixelShaderObject;
	}

	GuardianPixelShader::GuardianPixelShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
	{
		this->InitializePixelShader(graphics, shaderFilePath);
	}

	GuardianPixelShader::~GuardianPixelShader()
	{
	}

	void GuardianPixelShader::InitializePixelShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
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
		HRESULT hr = D3DCompile(ShaderCode.c_str(), ShaderCode.size(), null, null, null, "main", "ps_5_0", 0, 0, 
			this->PixelShaderBuffer.GetAddressOf(), null);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		hr = graphics->GetGraphicsDevice()->CreatePixelShader(this->PixelShaderBuffer->GetBufferPointer(),
			this->PixelShaderBuffer->GetBufferSize(), null, this->PixelShaderObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianPixelShader::Apply(std::shared_ptr<GuardianGraphics> graphics)
	{
		graphics->GetGraphicsDeviceContext()->PSSetShader(this->PixelShaderObject.Get(), null, 0);
	}

	WRL::ComPtr<ID3D11PixelShader> GuardianPixelShader::GetPixelShaderObject() noexcept
	{
		return this->PixelShaderObject;
	}

	WRL::ComPtr<ID3D10Blob> GuardianPixelShader::GetPixelShaderBuffer() noexcept
	{
		return this->PixelShaderBuffer;
	}

	std::shared_ptr<GuardianPixelShader> GuardianPixelShader::CreateNewPixelShader(
		std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath)
	{
		return std::make_shared<GuardianPixelShader>(graphics, shaderFilePath);
	}
}

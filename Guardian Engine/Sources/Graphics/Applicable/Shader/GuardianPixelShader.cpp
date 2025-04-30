#include "GuardianPixelShader.h"

namespace GE
{
	GuardianPixelShader::GuardianPixelShader(const GuardianPixelShader& other)
	{
		this->PixelShaderBuffer = other.PixelShaderBuffer;
		this->PixelShaderObject = other.PixelShaderObject;
	}

	GuardianPixelShader::GuardianPixelShader(const GString& shaderFilePath)
	{
		this->InitializePixelShader(shaderFilePath);
	}

	GuardianPixelShader::~GuardianPixelShader()
	{
	}

	void GuardianPixelShader::InitializePixelShader(const GString& shaderFilePath)
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
		HRESULT hr = D3DCompile(ShaderCode.c_str(), ShaderCode.size(), shaderFilePath.c_str(),
			null, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0,
			this->PixelShaderBuffer.GetAddressOf(), null);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		hr = GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDevice()->CreatePixelShader(this->PixelShaderBuffer->GetBufferPointer(),
			this->PixelShaderBuffer->GetBufferSize(), null, this->PixelShaderObject.GetAddressOf());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianPixelShader::Apply()
	{
		GuardianGraphicsRegistry::GetCurrentGraphics()->
			GetGraphicsDeviceContext()->PSSetShader(this->PixelShaderObject.Get(), null, 0);
	}

	WRL::ComPtr<ID3D11PixelShader> GuardianPixelShader::GetPixelShaderObject() noexcept
	{
		return this->PixelShaderObject;
	}

	WRL::ComPtr<ID3D10Blob> GuardianPixelShader::GetPixelShaderBuffer() noexcept
	{
		return this->PixelShaderBuffer;
	}

	std::shared_ptr<GuardianPixelShader> GuardianPixelShader::CreateNewPixelShader(const GString& shaderFilePath)
	{
		return std::make_shared<GuardianPixelShader>(shaderFilePath);
	}
}

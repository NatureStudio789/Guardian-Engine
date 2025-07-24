#include "GShader.h"

namespace GE
{
	GShader::GShader()
	{
		this->ShaderCategory = GE_NONE_SHADER;
	}

	GShader::GShader(Category category, std::string shaderFilePath, bool isBinaryFile)
	{
		this->InitializeShader(category, shaderFilePath, isBinaryFile);
	}

	GShader::GShader(const GShader& other)
	{
		this->ShaderCategory = other.ShaderCategory;
		this->ShaderByteCode = other.ShaderByteCode;
	}

	GShader::~GShader()
	{
		this->ShaderCategory = GE_NONE_SHADER;
	}

	void GShader::InitializeShader(Category category, std::string shaderFilePath, bool isBinaryFile)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->ShaderCategory = category;

		if (isBinaryFile)
		{
			std::ifstream ShaderFileStream(shaderFilePath, std::ios::binary);

			ShaderFileStream.seekg(0, std::ios_base::end);
			std::ifstream::pos_type size = (int)ShaderFileStream.tellg();
			ShaderFileStream.seekg(0, std::ios_base::beg);

			GUARDIAN_AUTO_THROW(D3DCreateBlob((SIZE_T)size, this->ShaderByteCode.GetAddressOf()));

			ShaderFileStream.read((char*)this->ShaderByteCode->GetBufferPointer(), (std::streamsize)size);
			ShaderFileStream.close();
		}
		else
		{
			std::string ShaderTarget;
			switch (this->ShaderCategory)
			{
				case GE_VERTEX_SHADER:
				{
					ShaderTarget = "vs_5_0";
					break;
				}

				case GE_PIXEL_SHADER:
				{
					ShaderTarget = "ps_5_0";
					break;
				}

				case GE_DOMAIN_SHADER:
				{
					ShaderTarget = "ds_5_0";
					break;
				}

				case GE_HULL_SHADER:
				{
					ShaderTarget = "hs_5_0";
					break;
				}

				case GE_GEOMETRY_SHADER:
				{
					ShaderTarget = "gs_5_0";
					break;
				}

				default:
				{
					ShaderTarget = "none_114514_114514";
					break;
				}
			}

			UINT CompileFlags = 0;
#ifdef GE_DEBUG
			CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			WRL::ComPtr<ID3DBlob> ShaderBlob;
			WRL::ComPtr<ID3DBlob> ErrorBlob;
			hr = D3DCompileFromFile(GUtil::StringToWideString(shaderFilePath).c_str(),
				null, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", ShaderTarget.c_str(), CompileFlags, 0,
				ShaderBlob.GetAddressOf(), ErrorBlob.GetAddressOf());

			if (ErrorBlob)
			{
				OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
			}

			GUARDIAN_AUTO_THROW(hr);

			this->ShaderByteCode = ShaderBlob;
		}
	}

	const GShader::Category& GShader::GetShaderCategory() const noexcept
	{
		return this->ShaderCategory;
	}

	WRL::ComPtr<ID3DBlob> GShader::GetShaderByteCode()
	{
		return this->ShaderByteCode;
	}
}

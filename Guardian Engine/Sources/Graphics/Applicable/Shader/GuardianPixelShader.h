#ifndef _GE_GUARDIANPIXELSHADER_H_
#define _GE_GUARDIANPIXELSHADER_H_
#include "../InputLayout/GuardianInputLayout.h"

namespace GE
{
	class GUARDIAN_API GuardianPixelShader : public GuardianApplicable
	{
	public:
		GuardianPixelShader() = default;
		GuardianPixelShader(const GuardianPixelShader& other);
		GuardianPixelShader(const GString& shaderFilePath);
		~GuardianPixelShader() override;

		void InitializePixelShader(const GString& shaderFilePath);

		void Apply() override;

		WRL::ComPtr<ID3D11PixelShader> GetPixelShaderObject() noexcept;
		WRL::ComPtr<ID3D10Blob> GetPixelShaderBuffer() noexcept;

		static std::shared_ptr<GuardianPixelShader> CreateNewPixelShader(const GString& shaderFilePath);

	private:
		WRL::ComPtr<ID3D11PixelShader> PixelShaderObject;
		WRL::ComPtr<ID3D10Blob> PixelShaderBuffer;
	};
}

#endif
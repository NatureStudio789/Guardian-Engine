#ifndef _GE_GUARDIANVERTEXSHADER_H_
#define _GE_GUARDIANVERTEXSHADER_H_
#include "../GuardianApplicable.h"

namespace GE
{
	class GUARDIAN_API GuardianVertexShader : public GuardianApplicable
	{
	public:
		GuardianVertexShader() = default;
		GuardianVertexShader(const GuardianVertexShader& other);
		GuardianVertexShader(const GString& shaderFilePath);
		~GuardianVertexShader() override;

		void InitializeVertexShader(const GString& shaderFilePath);

		void Apply() override;

		WRL::ComPtr<ID3D11VertexShader> GetVertexShaderObject() noexcept;
		WRL::ComPtr<ID3D10Blob> GetVertexShaderBuffer() noexcept;

		static std::shared_ptr<GuardianVertexShader> CreateNewVertexShader(const GString& shaderFilePath);

	private:
		WRL::ComPtr<ID3D11VertexShader> VertexShaderObject;
		WRL::ComPtr<ID3D10Blob> VertexShaderBuffer;
	};
}

#endif
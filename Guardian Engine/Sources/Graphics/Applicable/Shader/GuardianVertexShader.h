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
		GuardianVertexShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath);
		~GuardianVertexShader() override;

		void InitializeVertexShader(std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath);

		void Apply(std::shared_ptr<GuardianGraphics> graphics);

		WRL::ComPtr<ID3D11VertexShader> GetVertexShaderObject() noexcept;
		WRL::ComPtr<ID3D10Blob> GetVertexShaderBuffer() noexcept;

		static std::shared_ptr<GuardianVertexShader> CreateNewVertexShader(
			std::shared_ptr<GuardianGraphics> graphics, const GString& shaderFilePath);

	private:
		WRL::ComPtr<ID3D11VertexShader> VertexShaderObject;
		WRL::ComPtr<ID3D10Blob> VertexShaderBuffer;
	};
}

#endif
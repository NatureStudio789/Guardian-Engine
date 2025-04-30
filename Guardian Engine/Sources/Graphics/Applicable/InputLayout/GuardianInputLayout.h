#ifndef _GE_GUARDIANINPUTLAYOUT_H_
#define _GE_GUARDIANINPUTLAYOUT_H_
#include "../Shader/GuardianVertexShader.h"

namespace GE
{
	class GUARDIAN_API GuardianInputLayout : public GuardianApplicable
	{
	public:
		GuardianInputLayout() = default;
		GuardianInputLayout(const GuardianInputLayout& other);
		GuardianInputLayout(
			std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize);
		~GuardianInputLayout() override;

		void InitializeInputLayout(
			std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize);

		void Apply() override;

		WRL::ComPtr<ID3D11InputLayout> GetInputLayoutObject();

		static std::shared_ptr<GuardianInputLayout> CreateNewInputLayout(
			std::shared_ptr<GuardianVertexShader> vertexShader, D3D11_INPUT_ELEMENT_DESC data[], int dataSize);

	private:
		WRL::ComPtr<ID3D11InputLayout> InputLayoutObject;
	};
}

#endif
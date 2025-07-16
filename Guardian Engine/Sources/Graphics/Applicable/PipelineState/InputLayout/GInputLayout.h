#ifndef _GE_GINPUTLAYOUT_H_
#define _GE_GINPUTLAYOUT_H_
#include "../Shader/GShader.h"

namespace GE
{
	class GUARDIAN_API GInputLayout
	{
	public:
		GInputLayout();
		GInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescList, UINT elementCount);
		GInputLayout(const GInputLayout& other);
		~GInputLayout();

		void InitializeInputLayout(
			D3D12_INPUT_ELEMENT_DESC* inputElementDescList, UINT elementCount);

		const D3D12_INPUT_LAYOUT_DESC& GetInputLayoutDescription() const noexcept;
		const D3D12_INPUT_ELEMENT_DESC* GetInputElementList() const noexcept;
		const UINT& GetInputElementCount() const noexcept;

		static std::shared_ptr<GInputLayout> CreateNewInputLayout(
			D3D12_INPUT_ELEMENT_DESC* inputElementDescList, UINT elementCount)
		{
			return std::make_shared<GInputLayout>(inputElementDescList, elementCount);
		}

	private:
		D3D12_INPUT_LAYOUT_DESC InputLayoutDescription;
	};
}

#endif
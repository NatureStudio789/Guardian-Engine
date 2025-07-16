#include "GInputLayout.h"

namespace GE
{
    GInputLayout::GInputLayout()
    {
        GUARDIAN_CLEAR_MEMORY(this->InputLayoutDescription);
    }

    GInputLayout::GInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescList, UINT elementCount)
    {
        this->InitializeInputLayout(inputElementDescList, elementCount);
    }

    GInputLayout::GInputLayout(const GInputLayout& other)
    {
        this->InputLayoutDescription = other.InputLayoutDescription;
    }

    GInputLayout::~GInputLayout()
    {
        GUARDIAN_CLEAR_MEMORY(this->InputLayoutDescription);
    }

    void GInputLayout::InitializeInputLayout(D3D12_INPUT_ELEMENT_DESC* inputElementDescList, UINT elementCount)
    {
        this->InputLayoutDescription.pInputElementDescs = inputElementDescList;
        this->InputLayoutDescription.NumElements = elementCount;
    }

    const D3D12_INPUT_LAYOUT_DESC& GInputLayout::GetInputLayoutDescription() const noexcept
    {
        return this->InputLayoutDescription;
    }

    const D3D12_INPUT_ELEMENT_DESC* GInputLayout::GetInputElementList() const noexcept
    {
        return this->InputLayoutDescription.pInputElementDescs;
    }

    const UINT& GInputLayout::GetInputElementCount() const noexcept
    {
        return this->InputLayoutDescription.NumElements;
    }
}

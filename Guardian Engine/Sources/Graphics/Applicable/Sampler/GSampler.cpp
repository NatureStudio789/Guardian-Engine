#include "GSampler.h"

namespace GE
{
    GSampler::GSampler()
    {
        this->SamplerRootSignature = null;
        this->SamplerIndex = 0;
    }

    GSampler::GSampler(std::shared_ptr<GRootSignature> rootSignature, 
        const Filter& filter, const AddressMode& addressU, const AddressMode& addressV, const AddressMode& addressW, UINT index)
    {
        this->InitializeSampler(rootSignature, filter, addressU, addressV, addressW, index);
    }

    GSampler::GSampler(const GSampler& other)
    {
        this->SamplerRootSignature = other.SamplerRootSignature;
        this->SamplerIndex = other.SamplerIndex;
    }

    GSampler::~GSampler()
    {
        this->SamplerRootSignature = null;
        this->SamplerIndex = 0;
    }

    void GSampler::InitializeSampler(std::shared_ptr<GRootSignature> rootSignature, 
        const Filter& filter, const AddressMode& addressU, const AddressMode& addressV, const AddressMode& addressW, UINT index)
    {
        GUARDIAN_CHECK_POINTER(rootSignature);

        this->SamplerIndex = index;
        this->SamplerRootSignature = rootSignature;

        D3D12_SAMPLER_DESC SamplerDesc;
        GUARDIAN_CLEAR_MEMORY(SamplerDesc);
        SamplerDesc.Filter = (D3D12_FILTER)filter;
        SamplerDesc.AddressU = (D3D12_TEXTURE_ADDRESS_MODE)addressU;
        SamplerDesc.AddressV = (D3D12_TEXTURE_ADDRESS_MODE)addressV;
        SamplerDesc.AddressW = (D3D12_TEXTURE_ADDRESS_MODE)addressW;
        SamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
        SamplerDesc.MinLOD = 0.0f;
        SamplerDesc.MipLODBias = 0;
        SamplerDesc.MaxAnisotropy = 1;
        SamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

        GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
            CreateSampler(&SamplerDesc, this->GetSamplerCPUView());
    }

    void GSampler::Apply()
    {
        GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
            SetGraphicsRootDescriptorTable(this->SamplerIndex, this->GetSamplerGPUView());
    }
    
    CD3DX12_CPU_DESCRIPTOR_HANDLE GSampler::GetSamplerCPUView()
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
            this->SamplerRootSignature->GetTextureSamplerDescriptorHeap()->GetFirstCPUDescriptorHandle()).Offset(this->SamplerIndex,
            GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER));
    }

    CD3DX12_GPU_DESCRIPTOR_HANDLE GSampler::GetSamplerGPUView()
    {
        return CD3DX12_GPU_DESCRIPTOR_HANDLE(
            this->SamplerRootSignature->GetTextureSamplerDescriptorHeap()->GetFirstGPUDescriptorHandle()).Offset(this->SamplerIndex, 
                GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDescriptorSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER));
    }
}

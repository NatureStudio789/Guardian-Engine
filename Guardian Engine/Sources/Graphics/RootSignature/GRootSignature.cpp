#include "GRootSignature.h"

namespace GE
{
	GRootSignature::GRootSignature()
	{
		this->RootSignatureParameterList.clear();
		this->StaticSamplerDescList.clear();
		this->IsInitialized = false;
		this->DescriptorHeapList.clear();
	}

	GRootSignature::GRootSignature(const GRootSignature& other)
	{
		this->RootSignatureObject = other.RootSignatureObject;
		this->IsInitialized = other.IsInitialized;
		this->DescriptorHeapList = other.DescriptorHeapList;
		
		this->RootSignatureParameterList = other.RootSignatureParameterList;
		this->StaticSamplerDescList = other.StaticSamplerDescList;
	}

	GRootSignature::~GRootSignature()
	{
		this->StaticSamplerDescList.clear();
		this->IsInitialized = false;
		this->DescriptorHeapList.clear();
	}

	UINT GRootSignature::AddRootSignatureParameter(CD3DX12_ROOT_PARAMETER parameter)
	{
		if (this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has been initialized!");
		}

		UINT ParameterIndex = (UINT)this->RootSignatureParameterList.size();
		this->RootSignatureParameterList.push_back(parameter);

		return ParameterIndex;
	}

	void GRootSignature::AddStaticSamplerDesc(CD3DX12_STATIC_SAMPLER_DESC description)
	{
		if (this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has been initialized!");
		}

		this->StaticSamplerDescList.push_back(description);
	}

	void GRootSignature::InitializeRootSignature(std::shared_ptr<GDevice> device)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc((UINT)this->RootSignatureParameterList.size(), 
			this->RootSignatureParameterList.data(), 
			(UINT)this->StaticSamplerDescList.size(), 
			this->StaticSamplerDescList.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		WRL::ComPtr<ID3DBlob> SerializedRootSignature;
		WRL::ComPtr<ID3DBlob> ErrorBlob;
		GUARDIAN_AUTO_THROW(D3D12SerializeRootSignature(&RootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf()));
		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateRootSignature(0, SerializedRootSignature->GetBufferPointer(),
			SerializedRootSignature->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)this->RootSignatureObject.GetAddressOf()));

		this->IsInitialized = true;
	}

	void GRootSignature::AddDescriptorHeap(std::shared_ptr<GDescriptorHeap> descriptorHeap)
	{
		this->DescriptorHeapList.push_back(descriptorHeap);
	}

	void GRootSignature::SetRootDescriptorTable(std::shared_ptr<GCommandList> commandList, 
		UINT rootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle)
	{
		commandList->GetCommandListObject()->SetGraphicsRootDescriptorTable(rootParameterIndex, handle);
	}

	void GRootSignature::SetRootConstantBufferView(std::shared_ptr<GCommandList> commandList,
		UINT rootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress)
	{
		commandList->GetCommandListObject()->SetGraphicsRootConstantBufferView(rootParameterIndex, gpuVirtualAddress);
	}

	void GRootSignature::ApplyRootSignature(std::shared_ptr<GCommandList> commandList)
	{
		if (!this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has NOT been initialized!");
		}

		commandList->GetCommandListObject()->SetGraphicsRootSignature(this->RootSignatureObject.Get());

		std::vector<ID3D12DescriptorHeap*> DescriptorHeapObjectList;
		for (auto& descriptorHeap : this->DescriptorHeapList)
		{
			DescriptorHeapObjectList.push_back(descriptorHeap->GetDescriptorHeapObject().Get());
		}

		commandList->GetCommandListObject()->SetDescriptorHeaps((UINT)DescriptorHeapObjectList.size(),
			DescriptorHeapObjectList.data());
	}

	WRL::ComPtr<ID3D12RootSignature> GRootSignature::GetRootSignatureObject()
	{
		return this->RootSignatureObject;
	}

	const bool& GRootSignature::GetInitialized() const noexcept
	{
		return this->IsInitialized;
	}
}

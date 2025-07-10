#include "GRootSignature.h"

namespace GE
{
	GRootSignature::GRootSignature()
	{
		this->DescriptorHeapList.clear();
	}

	GRootSignature::GRootSignature(std::shared_ptr<GDevice> device,
		std::vector<CD3DX12_ROOT_PARAMETER> rootSignatureParameters)
	{
		this->InitializeRootSignature(device, rootSignatureParameters);
	}

	GRootSignature::GRootSignature(const GRootSignature& other)
	{
		this->RootSignatureObject = other.RootSignatureObject;
		this->DescriptorHeapList = other.DescriptorHeapList;
	}

	GRootSignature::~GRootSignature()
	{
		this->DescriptorHeapList.clear();
	}

	void GRootSignature::InitializeRootSignature(std::shared_ptr<GDevice> device,
		std::vector<CD3DX12_ROOT_PARAMETER> rootSignatureParameters)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc((UINT)rootSignatureParameters.size(), rootSignatureParameters.data(), 0, null, 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		WRL::ComPtr<ID3DBlob> SerializedRootSignature;
		WRL::ComPtr<ID3DBlob> ErrorBlob;
		GUARDIAN_AUTO_THROW(D3D12SerializeRootSignature(&RootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf()));
		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateRootSignature(0, SerializedRootSignature->GetBufferPointer(),
			SerializedRootSignature->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)this->RootSignatureObject.GetAddressOf()));
	}

	void GRootSignature::AddDescriptorHeap(std::shared_ptr<GDescriptorHeap> descriptorHeap)
	{
		GUARDIAN_CHECK_POINTER(descriptorHeap);

		this->DescriptorHeapList.push_back(descriptorHeap);
	}

	void GRootSignature::ApplyRootSignature(std::shared_ptr<GCommandList> commandList, 
		UINT index, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorHandle)
	{
		commandList->GetCommandListObject()->SetGraphicsRootSignature(this->RootSignatureObject.Get());
		std::vector<ID3D12DescriptorHeap*> D3DDescriptorHeap;
		for (auto& descriptorHeap : this->DescriptorHeapList)
		{
			D3DDescriptorHeap.push_back(descriptorHeap->GetDescriptorHeapObject().Get());
		}

		commandList->GetCommandListObject()->SetDescriptorHeaps((UINT)D3DDescriptorHeap.size(), D3DDescriptorHeap.data());

		commandList->GetCommandListObject()->SetGraphicsRootDescriptorTable(index, descriptorHandle);
	}

	WRL::ComPtr<ID3D12RootSignature> GRootSignature::GetRootSignatureObject()
	{
		return this->RootSignatureObject;
	}
}

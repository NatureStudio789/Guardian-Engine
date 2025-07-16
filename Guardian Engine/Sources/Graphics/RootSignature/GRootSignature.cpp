#include "GRootSignature.h"

namespace GE
{
	GRootSignature::GRootSignature()
	{
		this->RootParameterList.clear();
		this->SamplerDescList.clear();
		this->IsInitialized = false;
		this->DescriptorHeapList.clear();
	}

	GRootSignature::GRootSignature(const GRootSignature& other)
	{
		this->RootSignatureObject = other.RootSignatureObject;
		this->IsInitialized = other.IsInitialized;
		this->DescriptorHeapList = other.DescriptorHeapList;
		
		this->RootParameterList = other.RootParameterList;
		this->SamplerDescList = other.SamplerDescList;
	}

	GRootSignature::~GRootSignature()
	{
		this->SamplerDescList.clear();
		this->IsInitialized = false;
		this->DescriptorHeapList.clear();
	}

	UINT GRootSignature::AddRootParameter(const RootParameter& parameter)
	{
		if (this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has been initialized!");
		}

		UINT ParameterIndex = (UINT)this->RootParameterList.size();
		this->RootParameterList.push_back(parameter);

		return ParameterIndex;
	}

	void GRootSignature::AddSamplerDescription(const SamplerDescription& description)
	{
		if (this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has been initialized!");
		}

		this->SamplerDescList.push_back(description);
	}

	void GRootSignature::InitializeRootSignature(std::shared_ptr<GDevice> device)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		std::vector<CD3DX12_ROOT_PARAMETER> ParameterList;
		for (auto& parameter : this->RootParameterList)
		{
			CD3DX12_ROOT_PARAMETER Parameter;

			switch (parameter.Type)
			{
				case GE_PARAMETER_CBV:
				{
					CD3DX12_DESCRIPTOR_RANGE ConstantBufferViewTable;
					ConstantBufferViewTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, parameter.ShaderRegisterIndex);

					Parameter.InitAsDescriptorTable(1, &ConstantBufferViewTable);

					break;
				}
			}

			ParameterList.push_back(Parameter);
		}

		std::vector<CD3DX12_STATIC_SAMPLER_DESC> DescriptionList;
		for (auto& description : this->SamplerDescList)
		{
			CD3DX12_STATIC_SAMPLER_DESC Description;

			Description.Filter = description.Filter;
			Description.AddressU = description.AddressU;
			Description.AddressV = description.AddressV;
			Description.AddressW = description.AddressW;
			Description.MipLODBias = description.MipLODBias;
			Description.MaxAnisotropy = description.MaxAnisotropy;
			Description.ComparisonFunc = description.ComparisonFunc;
			Description.BorderColor = description.BorderColor;
			Description.MinLOD = description.MinLOD;
			Description.MaxLOD = description.MaxLOD;
			Description.ShaderRegister = description.ShaderRegister;
			Description.RegisterSpace = description.RegisterSpace;
			Description.ShaderVisibility = description.ShaderVisibility;

			DescriptionList.push_back(Description);
		}

		CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc((UINT)ParameterList.size(), 
			ParameterList.data(), 
			(UINT)DescriptionList.size(), 
			DescriptionList.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		WRL::ComPtr<ID3DBlob> SerializedRootSignature;
		WRL::ComPtr<ID3DBlob> ErrorBlob;
		hr = D3D12SerializeRootSignature(&RootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1, SerializedRootSignature.GetAddressOf(), ErrorBlob.GetAddressOf());

		if (ErrorBlob)
		{
			::OutputDebugStringA((char*)ErrorBlob->GetBufferPointer());
		}
		GUARDIAN_AUTO_THROW(hr);

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

	void GRootSignature::SetDescriptorHeapList(std::shared_ptr<GCommandList> commandList)
	{
		std::vector<ID3D12DescriptorHeap*> DescriptorHeapObjectList;
		for (auto& descriptorHeap : this->DescriptorHeapList)
		{
			DescriptorHeapObjectList.push_back(descriptorHeap->GetDescriptorHeapObject().Get());
		}

		commandList->GetCommandListObject()->SetDescriptorHeaps((UINT)DescriptorHeapObjectList.size(),
			DescriptorHeapObjectList.data());
	}

	void GRootSignature::ApplyRootSignature(std::shared_ptr<GCommandList> commandList)
	{
		if (!this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has NOT been initialized!");
		}

		commandList->GetCommandListObject()->SetGraphicsRootSignature(this->RootSignatureObject.Get());
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

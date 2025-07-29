#include "GRootSignature.h"

namespace GE
{
	GRootSignature::GRootSignature()
	{
		this->RootParameterList.clear();
		this->StaticSamplerDescList.clear();
		this->IsInitialized = false;
		this->CBVSRVDescriptorHeap = null;
		this->TextureSamplerDescriptorHeap = null;
	}

	GRootSignature::GRootSignature(const GRootSignature& other)
	{
		this->RootSignatureObject = other.RootSignatureObject;
		this->IsInitialized = other.IsInitialized;
		this->CBVSRVDescriptorHeap = other.CBVSRVDescriptorHeap;
		this->TextureSamplerDescriptorHeap = other.TextureSamplerDescriptorHeap;
		
		this->RootParameterList = other.RootParameterList;
		this->StaticSamplerDescList = other.StaticSamplerDescList;
	}

	GRootSignature::~GRootSignature()
	{
		this->StaticSamplerDescList.clear();
		this->IsInitialized = false;

		this->CBVSRVDescriptorHeap.reset();
		this->CBVSRVDescriptorHeap = null;

		this->TextureSamplerDescriptorHeap.reset();
		this->TextureSamplerDescriptorHeap = null;
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

	void GRootSignature::AddSamplerDescription(const StaticSamplerDescription& description)
	{
		if (this->IsInitialized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This root signature has been initialized!");
		}

		this->StaticSamplerDescList.push_back(description);
	}

	void GRootSignature::InitializeRootSignature(std::shared_ptr<GDevice> device, 
		UINT cbvDescriptorCount, UINT samplerDescriptorCount, UINT srvDescriptorCount)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		if (cbvDescriptorCount || srvDescriptorCount)
		{
			this->CBVSRVDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(device, cbvDescriptorCount + srvDescriptorCount,
				GDescriptorHeap::GE_DESCRIPTOR_HEAP_CBVSRVUAV, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);
		}

		if (samplerDescriptorCount)
		{
			this->TextureSamplerDescriptorHeap = GDescriptorHeap::CreateNewDescriptorHeap(device, samplerDescriptorCount,
				GDescriptorHeap::GE_DESCRIPTOR_HEAP_SAMPLER, GDescriptorHeap::GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE);
		}

		std::vector<CD3DX12_ROOT_PARAMETER> ParameterList;
		for (auto& parameter : this->RootParameterList)
		{
			CD3DX12_ROOT_PARAMETER Parameter;

			switch (parameter.Type)
			{
				case GE_PARAMETER_CBV:
				{
					Parameter.InitAsConstantBufferView(parameter.ShaderRegisterIndex);

					break;
				}

				case GE_PARAMETER_SRV:
				{
					CD3DX12_DESCRIPTOR_RANGE ShaderResourceViewTable;
					ShaderResourceViewTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, parameter.ShaderRegisterIndex);

					Parameter.InitAsDescriptorTable(1, &ShaderResourceViewTable, D3D12_SHADER_VISIBILITY_PIXEL);
					break;
				}
			}

			ParameterList.push_back(Parameter);
		}

		std::vector<CD3DX12_STATIC_SAMPLER_DESC> DescriptionList;
		for (auto& description : this->StaticSamplerDescList)
		{
			CD3DX12_STATIC_SAMPLER_DESC Description;

			Description = CD3DX12_STATIC_SAMPLER_DESC(description.ShaderRegister, description.Filter,
				description.AddressU, description.AddressV, description.AddressW);

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
		if (this->CBVSRVDescriptorHeap)
		{
			DescriptorHeapObjectList.push_back(this->CBVSRVDescriptorHeap->GetDescriptorHeapObject().Get());
		}
		if (this->TextureSamplerDescriptorHeap)
		{
			DescriptorHeapObjectList.push_back(this->TextureSamplerDescriptorHeap->GetDescriptorHeapObject().Get());
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

		this->SetDescriptorHeapList(commandList);
	}

	const UINT GRootSignature::GetRootParameterIndex(const RootParameter& parameter) const
	{
		for (UINT i = 0; i < (UINT)this->RootParameterList.size(); i++)
		{
			if (parameter == this->RootParameterList[i])
			{
				return i;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("No suitable root parameter found!");
	}

	const UINT GRootSignature::GetStaticSamplerIndex(const StaticSamplerDescription& description) const
	{
		for (UINT i = 0; i < (UINT)this->StaticSamplerDescList.size(); i++)
		{
			if (description == this->StaticSamplerDescList[i])
			{
				return i;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("No suitable static sampler description found!");
	}

	WRL::ComPtr<ID3D12RootSignature> GRootSignature::GetRootSignatureObject()
	{
		return this->RootSignatureObject;
	}

	std::shared_ptr<GDescriptorHeap> GRootSignature::GetConstantBufferDescriptorHeap()
	{
		return this->CBVSRVDescriptorHeap;
	}

	std::shared_ptr<GDescriptorHeap> GRootSignature::GetTextureSamplerDescriptorHeap()
	{
		return this->TextureSamplerDescriptorHeap;
	}

	std::shared_ptr<GDescriptorHeap> GRootSignature::GetShaderResourceDescriptorHeap()
	{
		return this->CBVSRVDescriptorHeap;
	}

	const bool& GRootSignature::GetInitialized() const noexcept
	{
		return this->IsInitialized;
	}
}

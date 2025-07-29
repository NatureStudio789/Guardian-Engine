#ifndef _GE_GROOTSIGNATURE_H_
#define _GE_GROOTSIGNATURE_H_
#include "../DescriptorHeap/GDescriptorHeap.h"

namespace GE
{
	class GUARDIAN_API GRootSignature
	{
	public:
		enum ParameterType
		{
			GE_PARAMETER_CBV,
			GE_PARAMETER_SRV
		};

		struct RootParameter
		{
			RootParameter()
			{
				this->Type = GE_PARAMETER_CBV;
				this->ShaderRegisterIndex = 0;
			}
			RootParameter(const ParameterType& type, UINT index)
			{
				this->Type = type;
				this->ShaderRegisterIndex = index;
			}
			RootParameter(const RootParameter& other)
			{
				this->Type = other.Type;
				this->ShaderRegisterIndex = other.ShaderRegisterIndex;
			}

			bool operator==(const RootParameter& other) const noexcept
			{
				return this->Type == other.Type && this->ShaderRegisterIndex == other.ShaderRegisterIndex;
			}

			ParameterType Type;
			UINT ShaderRegisterIndex;
		};

		struct StaticSamplerDescription
		{
			StaticSamplerDescription() = default;
			StaticSamplerDescription(UINT index, D3D12_FILTER filter,
			D3D12_TEXTURE_ADDRESS_MODE addressU,
			D3D12_TEXTURE_ADDRESS_MODE addressV,
			D3D12_TEXTURE_ADDRESS_MODE addressW)
			{
				this->ShaderRegister = index;
				this->Filter = filter;
				this->AddressU = addressU;
				this->AddressV = addressV;
				this->AddressW = addressW;
			}
			StaticSamplerDescription(const StaticSamplerDescription& other)
			{
				this->ShaderRegister = other.ShaderRegister;
				this->Filter = other.Filter;
				this->AddressU = other.AddressU;
				this->AddressV = other.AddressV;
				this->AddressW = other.AddressW;
			}

			bool operator==(const StaticSamplerDescription& other) const noexcept
			{
				return this->ShaderRegister == other.ShaderRegister && this->Filter == other.Filter && this->AddressU == other.AddressU
					&& this->AddressV == other.AddressV && this->AddressW == this->AddressW;
			}

			UINT ShaderRegister;
			D3D12_FILTER Filter;
			D3D12_TEXTURE_ADDRESS_MODE AddressU;
			D3D12_TEXTURE_ADDRESS_MODE AddressV;
			D3D12_TEXTURE_ADDRESS_MODE AddressW;
		};

	public:
		GRootSignature();
		GRootSignature(const GRootSignature& other);
		~GRootSignature();

		UINT AddRootParameter(const RootParameter& parameter);
		void AddSamplerDescription(const StaticSamplerDescription& description);
		void InitializeRootSignature(std::shared_ptr<GDevice> device, 
			UINT cbvDescriptorCount, UINT samplerDescriptorCount, UINT srvDescriptorCount);

		void SetRootDescriptorTable(std::shared_ptr<GCommandList> commandList,
			UINT rootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootConstantBufferView(std::shared_ptr<GCommandList> commandList, 
			UINT rootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress);
		
		void ApplyRootSignature(std::shared_ptr<GCommandList> commandList);

		const UINT GetRootParameterIndex(const RootParameter& parameter) const;
		const UINT GetStaticSamplerIndex(const StaticSamplerDescription& description) const;
		WRL::ComPtr<ID3D12RootSignature> GetRootSignatureObject();
		std::shared_ptr<GDescriptorHeap> GetConstantBufferDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetTextureSamplerDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetShaderResourceDescriptorHeap();
		const bool& GetInitialized() const noexcept;

	private:
		void SetDescriptorHeapList(std::shared_ptr<GCommandList> commandList);

		WRL::ComPtr<ID3D12RootSignature> RootSignatureObject;
		bool IsInitialized;
		std::shared_ptr<GDescriptorHeap> CBVSRVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> TextureSamplerDescriptorHeap;

		std::vector<RootParameter> RootParameterList;
		std::vector<StaticSamplerDescription> StaticSamplerDescList;
	};
}

#endif
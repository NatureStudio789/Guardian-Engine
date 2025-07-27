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
			RootParameter(const RootParameter& other)
			{
				this->Type = other.Type;
				this->ShaderRegisterIndex = other.ShaderRegisterIndex;
			}

			ParameterType Type;
			UINT ShaderRegisterIndex;
		};

		struct SamplerDescription
		{
			SamplerDescription() = default;
			SamplerDescription(const SamplerDescription& other)
			{
				this->ShaderRegister = other.ShaderRegister;
				this->Filter = other.Filter;
				this->AddressU = other.AddressU;
				this->AddressV = other.AddressV;
				this->AddressW = other.AddressW;
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
		void AddSamplerDescription(const SamplerDescription& description);
		void InitializeRootSignature(std::shared_ptr<GDevice> device, 
			UINT cbvDescriptorCount, UINT samplerDescriptorCount, UINT srvDescriptorCount);

		void SetRootDescriptorTable(std::shared_ptr<GCommandList> commandList,
			UINT rootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootConstantBufferView(std::shared_ptr<GCommandList> commandList, 
			UINT rootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress);
		
		void ApplyRootSignature(std::shared_ptr<GCommandList> commandList);

		WRL::ComPtr<ID3D12RootSignature> GetRootSignatureObject();
		std::shared_ptr<GDescriptorHeap> GetConstantBufferDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetTextureSamplerDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetShaderResourceDescriptorHeap();
		const bool& GetInitialized() const noexcept;

	private:
		void SetDescriptorHeapList(std::shared_ptr<GCommandList> commandList);

		WRL::ComPtr<ID3D12RootSignature> RootSignatureObject;
		bool IsInitialized;
		std::shared_ptr<GDescriptorHeap> ConstantBufferDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> TextureSamplerDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> ShaderResourceDescriptorHeap;

		std::vector<RootParameter> RootParameterList;
		std::vector<SamplerDescription> SamplerDescList;
	};
}

#endif
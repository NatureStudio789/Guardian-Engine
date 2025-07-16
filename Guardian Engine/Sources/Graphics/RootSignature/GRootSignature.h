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
			GE_PARAMETER_CBV
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
				this->Filter = other.Filter;
				this->AddressU = other.AddressU;
				this->AddressV = other.AddressV;
				this->AddressW = other.AddressW;
				this->MipLODBias = other.MipLODBias;
				this->MaxAnisotropy = other.MaxAnisotropy;
				this->ComparisonFunc = other.ComparisonFunc;
				this->BorderColor = other.BorderColor;
				this->MinLOD = other.MinLOD;
				this->MaxLOD = other.MaxLOD;
				this->ShaderRegister = other.ShaderRegister;
				this->RegisterSpace = other.RegisterSpace;
				this->ShaderVisibility = other.ShaderVisibility;
			}

			D3D12_FILTER Filter;
			D3D12_TEXTURE_ADDRESS_MODE AddressU;
			D3D12_TEXTURE_ADDRESS_MODE AddressV;
			D3D12_TEXTURE_ADDRESS_MODE AddressW;
			FLOAT MipLODBias;
			UINT MaxAnisotropy;
			D3D12_COMPARISON_FUNC ComparisonFunc;
			D3D12_STATIC_BORDER_COLOR BorderColor;
			FLOAT MinLOD;
			FLOAT MaxLOD;
			UINT ShaderRegister;
			UINT RegisterSpace;
			D3D12_SHADER_VISIBILITY ShaderVisibility;
		};

	public:
		GRootSignature();
		GRootSignature(const GRootSignature& other);
		~GRootSignature();

		UINT AddRootParameter(const RootParameter& parameter);
		void AddSamplerDescription(const SamplerDescription& description);
		void InitializeRootSignature(std::shared_ptr<GDevice> device);

		void AddDescriptorHeap(std::shared_ptr<GDescriptorHeap> descriptorHeap);

		void SetRootDescriptorTable(std::shared_ptr<GCommandList> commandList,
			UINT rootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootConstantBufferView(std::shared_ptr<GCommandList> commandList, 
			UINT rootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress);
		
		void SetDescriptorHeapList(std::shared_ptr<GCommandList> commandList);
		void ApplyRootSignature(std::shared_ptr<GCommandList> commandList);

		WRL::ComPtr<ID3D12RootSignature> GetRootSignatureObject();
		const bool& GetInitialized() const noexcept;

	private:
		WRL::ComPtr<ID3D12RootSignature> RootSignatureObject;
		bool IsInitialized;
		std::vector<std::shared_ptr<GDescriptorHeap>> DescriptorHeapList;

		std::vector<RootParameter> RootParameterList;
		std::vector<SamplerDescription> SamplerDescList;
	};
}

#endif
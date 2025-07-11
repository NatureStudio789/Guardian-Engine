#ifndef _GE_GROOTSIGNATURE_H_
#define _GE_GROOTSIGNATURE_H_
#include "../DescriptorHeap/GDescriptorHeap.h"

namespace GE
{
	class GUARDIAN_API GRootSignature
	{
	public:
		GRootSignature();
		GRootSignature(const GRootSignature& other);
		~GRootSignature();

		UINT AddRootSignatureParameter(CD3DX12_ROOT_PARAMETER parameter);
		void AddStaticSamplerDesc(CD3DX12_STATIC_SAMPLER_DESC description);
		void InitializeRootSignature(std::shared_ptr<GDevice> device);

		void AddDescriptorHeap(std::shared_ptr<GDescriptorHeap> descriptorHeap);

		void SetRootDescriptorTable(std::shared_ptr<GCommandList> commandList,
			UINT rootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);
		void SetRootConstantBufferView(std::shared_ptr<GCommandList> commandList, 
			UINT rootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress);
		
		void ApplyRootSignature(std::shared_ptr<GCommandList> commandList);

		WRL::ComPtr<ID3D12RootSignature> GetRootSignatureObject();
		const bool& GetInitialized() const noexcept;

	private:
		WRL::ComPtr<ID3D12RootSignature> RootSignatureObject;
		bool IsInitialized;
		std::vector<std::shared_ptr<GDescriptorHeap>> DescriptorHeapList;

		std::vector<CD3DX12_ROOT_PARAMETER> RootSignatureParameterList;
		std::vector<CD3DX12_STATIC_SAMPLER_DESC> StaticSamplerDescList;
	};
}

#endif
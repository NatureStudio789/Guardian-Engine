#ifndef _GE_GROOTSIGNATURE_H_
#define _GE_GROOTSIGNATURE_H_
#include "../DescriptorHeap/GDescriptorHeap.h"

namespace GE
{
	class GUARDIAN_API GRootSignature
	{
	public:
		GRootSignature();
		GRootSignature(std::shared_ptr<GDevice> device, 
			std::vector<CD3DX12_ROOT_PARAMETER> rootSignatureParameters);
		GRootSignature(const GRootSignature& other);
		~GRootSignature();

		void InitializeRootSignature(std::shared_ptr<GDevice> device,
			std::vector<CD3DX12_ROOT_PARAMETER> rootSignatureParameters);
		void AddDescriptorHeap(std::shared_ptr<GDescriptorHeap> descriptorHeap);

		void ApplyRootSignature(std::shared_ptr<GCommandList> commandList,
			UINT index, CD3DX12_GPU_DESCRIPTOR_HANDLE descriptorHandle);

		WRL::ComPtr<ID3D12RootSignature> GetRootSignatureObject();

		static std::shared_ptr<GRootSignature> CreateNewRootSignature(std::shared_ptr<GDevice> device,
			std::vector<CD3DX12_ROOT_PARAMETER> rootSignatureParameters)
		{
			return std::make_shared<GRootSignature>(device, rootSignatureParameters);
		}

	private:
		WRL::ComPtr<ID3D12RootSignature> RootSignatureObject;

		std::vector<std::shared_ptr<GDescriptorHeap>> DescriptorHeapList;
	};
}

#endif
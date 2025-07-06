#ifndef _GE_GDESCRIPTORHEAP_H_
#define _GE_GDESCRIPTORHEAP_H_
#include "../SwapChain/GSwapChain.h"

namespace GE
{
	class GUARDIAN_API GDescriptorHeap
	{
	public:
		enum Category
		{
			GE_DESCRIPTOR_HEAP_RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			GE_DESCRIPTOR_HEAP_DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};

	public:
		GDescriptorHeap();
		GDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, Category category);
		GDescriptorHeap(const GDescriptorHeap& other);
		~GDescriptorHeap();

		void InitializeDescriptorHeap(
			std::shared_ptr<GDevice> device, UINT descriptorCount, Category category);

		D3D12_CPU_DESCRIPTOR_HANDLE GetFirstCPUDescriptorHandle();
		WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeapObject();

		static std::shared_ptr<GDescriptorHeap> CreateNewDescriptorHeap(
			std::shared_ptr<GDevice> device, UINT descriptorCount, Category category)
		{
			return std::make_shared<GDescriptorHeap>(device, descriptorCount, category);
		}

	private:
		WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapObject;
	};
}

#endif
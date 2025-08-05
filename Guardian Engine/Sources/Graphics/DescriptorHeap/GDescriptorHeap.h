#ifndef _GE_GDESCRIPTORHEAP_H_
#define _GE_GDESCRIPTORHEAP_H_
#include "GBitmapAllocator.h"

namespace GE
{
	class GUARDIAN_API GDescriptorHandle
	{
	public:
		GDescriptorHandle()
		{
			this->CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE();
			this->GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE();
		}
		GDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
		{
			this->CPUHandle = cpuHandle;
			this->GPUHandle = gpuHandle;
		}
		GDescriptorHandle(const GDescriptorHandle& other)
		{
			this->CPUHandle = other.CPUHandle;
			this->GPUHandle = other.GPUHandle;
		}

		static std::shared_ptr<GDescriptorHandle> CreateNewDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
		{
			return std::make_shared<GDescriptorHandle>(cpuHandle, gpuHandle);
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle;
		CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle;
	};

	class GUARDIAN_API GDescriptorHeap
	{
	public:
		enum Category
		{
			GE_DESCRIPTOR_HEAP_CBVSRVUAV = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			GE_DESCRIPTOR_HEAP_SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
			GE_DESCRIPTOR_HEAP_RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			GE_DESCRIPTOR_HEAP_DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		};

		enum Flag
		{
			GE_DESCRIPTOR_HEAP_FLAG_NONE = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			GE_DESCRIPTOR_HEAP_FLAG_SHADERVISIBLE = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		};

	public:
		GDescriptorHeap();
		GDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, 
			Category category, Flag flag = GE_DESCRIPTOR_HEAP_FLAG_NONE);
		GDescriptorHeap(const GDescriptorHeap& other);
		~GDescriptorHeap();

		void InitializeDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, 
			Category category, Flag flag = GE_DESCRIPTOR_HEAP_FLAG_NONE);

		std::shared_ptr<GDescriptorHandle> Allocate(UINT descriptorCount);

		WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeapObject();

		static std::shared_ptr<GDescriptorHeap> CreateNewDescriptorHeap(std::shared_ptr<GDevice> device, 
			UINT descriptorCount, Category category, Flag flag = GE_DESCRIPTOR_HEAP_FLAG_NONE)
		{
			return std::make_shared<GDescriptorHeap>(device, descriptorCount, category, flag);
		}

	private:
		WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapObject;
		std::shared_ptr<GBitmapAllocator> DescriptorHeapAllocator;

		UINT DescriptorIncrementSize;
	};
}

#endif
#include "GDescriptorHeap.h"

namespace GE
{
	GDescriptorHeap::GDescriptorHeap()
	{
		this->DescriptorHeapAllocator = null;
		this->DescriptorIncrementSize = 0;
	}

	GDescriptorHeap::GDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, 
		Category category, Flag flag)
	{
		this->InitializeDescriptorHeap(device, descriptorCount, category, flag);
	}

	GDescriptorHeap::GDescriptorHeap(const GDescriptorHeap& other)
	{
		this->DescriptorHeapObject = other.DescriptorHeapObject;
		this->DescriptorHeapAllocator = other.DescriptorHeapAllocator;

		this->DescriptorIncrementSize = other.DescriptorIncrementSize;
	}

	GDescriptorHeap::~GDescriptorHeap()
	{
		this->DescriptorHeapAllocator.reset();
		this->DescriptorIncrementSize = 0;
	}

	void GDescriptorHeap::InitializeDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, 
		Category category, Flag flag)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		this->DescriptorHeapAllocator = GBitmapAllocator::CreateNewBitmapAllocator(descriptorCount);

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;
		GUARDIAN_CLEAR_MEMORY(DescriptorHeapDesc);
		DescriptorHeapDesc.NumDescriptors = descriptorCount;
		DescriptorHeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)category;
		DescriptorHeapDesc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)flag;
		DescriptorHeapDesc.NodeMask = 0;

		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateDescriptorHeap(&DescriptorHeapDesc,
			__uuidof(ID3D12DescriptorHeap), (void**)this->DescriptorHeapObject.GetAddressOf()));

		this->DescriptorIncrementSize = device->GetDescriptorSize((D3D12_DESCRIPTOR_HEAP_TYPE)category);
	}

	std::shared_ptr<GDescriptorHandle> GDescriptorHeap::Allocate(UINT descriptorCount)
	{
		UINT StartIndex = this->DescriptorHeapAllocator->Allocate(descriptorCount);

		CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(this->DescriptorHeapObject->GetCPUDescriptorHandleForHeapStart());
		CPUHandle.Offset(StartIndex, this->DescriptorIncrementSize);

		CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle = {};
		if (this->DescriptorHeapObject->GetDesc().Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		{
			GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(this->DescriptorHeapObject->GetGPUDescriptorHandleForHeapStart());
			GPUHandle.Offset(StartIndex, this->DescriptorIncrementSize);
		}

		return GDescriptorHandle::CreateNewDescriptorHandle(CPUHandle, GPUHandle);
	}

	WRL::ComPtr<ID3D12DescriptorHeap> GDescriptorHeap::GetDescriptorHeapObject()
	{
		return this->DescriptorHeapObject;
	}
}

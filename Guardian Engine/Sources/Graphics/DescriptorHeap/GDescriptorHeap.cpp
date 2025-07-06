#include "GDescriptorHeap.h"

namespace GE
{
	GDescriptorHeap::GDescriptorHeap()
	{

	}

	GDescriptorHeap::GDescriptorHeap(std::shared_ptr<GDevice> device, UINT descriptorCount, Category category)
	{
		this->InitializeDescriptorHeap(device, descriptorCount, category);
	}

	GDescriptorHeap::GDescriptorHeap(const GDescriptorHeap& other)
	{
		this->DescriptorHeapObject = other.DescriptorHeapObject;
	}

	GDescriptorHeap::~GDescriptorHeap()
	{

	}

	void GDescriptorHeap::InitializeDescriptorHeap(
		std::shared_ptr<GDevice> device, UINT descriptorCount, Category category)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc;
		GUARDIAN_CLEAR_MEMORY(DescriptorHeapDesc);
		DescriptorHeapDesc.NumDescriptors = descriptorCount;
		DescriptorHeapDesc.Type = (D3D12_DESCRIPTOR_HEAP_TYPE)category;
		DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DescriptorHeapDesc.NodeMask = 0;

		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateDescriptorHeap(&DescriptorHeapDesc,
			__uuidof(ID3D12DescriptorHeap), (void**)this->DescriptorHeapObject.GetAddressOf()));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE GDescriptorHeap::GetFirstCPUDescriptorHandle()
	{
		GUARDIAN_CHECK_VALUE((long long)this->DescriptorHeapObject.Get());
		return this->DescriptorHeapObject->GetCPUDescriptorHandleForHeapStart();
	}

	WRL::ComPtr<ID3D12DescriptorHeap> GDescriptorHeap::GetDescriptorHeapObject()
	{
		return this->DescriptorHeapObject;
	}
}

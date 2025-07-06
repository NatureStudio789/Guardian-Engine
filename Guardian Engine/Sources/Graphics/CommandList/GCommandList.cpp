#include "GCommandList.h"

namespace GE
{
	GCommandList::GCommandList()
	{

	}

	GCommandList::GCommandList(std::shared_ptr<GDevice> device)
	{
		this->InitializeCommandList(device);
	}

	GCommandList::GCommandList(const GCommandList& other)
	{
		this->CommandListAllocator = other.CommandListAllocator;
		this->CommandListObject = other.CommandListObject;
	}

	GCommandList::~GCommandList()
	{

	}

	void GCommandList::InitializeCommandList(std::shared_ptr<GDevice> device)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			__uuidof(ID3D12CommandAllocator), (void**)this->CommandListAllocator.GetAddressOf()));
		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			this->CommandListAllocator.Get(), null, __uuidof(ID3D12GraphicsCommandList), (void**)this->CommandListObject.GetAddressOf()));

		GUARDIAN_AUTO_THROW(this->CommandListObject->Close());
	}

	WRL::ComPtr<ID3D12CommandAllocator> GCommandList::GetCommandListAllocator()
	{
		return WRL::ComPtr<ID3D12CommandAllocator>();
	}

	WRL::ComPtr<ID3D12GraphicsCommandList> GCommandList::GetCommandListObject()
	{
		return this->CommandListObject;
	}
}

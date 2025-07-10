#include "GFence.h"

namespace GE
{
	GFence::GFence()
	{
		this->FenceValue = 0;
	}

	GFence::GFence(std::shared_ptr<GDevice> device)
	{
		this->InitializeFence(device);
	}

	GFence::GFence(const GFence& other)
	{
		this->FenceValue = other.FenceValue;
		this->FenceObject = other.FenceObject;
	}

	GFence::~GFence()
	{
		this->FenceValue = 0;
	}

	void GFence::InitializeFence(std::shared_ptr<GDevice> device)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->FenceValue = 0;
		
		GUARDIAN_AUTO_THROW(device->GetDeviceObject()->CreateFence(0, D3D12_FENCE_FLAG_NONE,
			__uuidof(ID3D12Fence), (void**)this->FenceObject.GetAddressOf()));
	}

	void GFence::FlushFence(std::shared_ptr<GCommandQueue> commandQueue)
	{
		GUARDIAN_SETUP_AUTO_THROW();
		this->FenceValue++;

		GUARDIAN_AUTO_THROW(commandQueue->GetCommandQueueObject()->Signal(this->FenceObject.Get(), this->FenceValue));

		if (this->FenceObject->GetCompletedValue() < this->FenceValue)
		{
			HANDLE EventHandle = CreateEventEx(null, null, 0, EVENT_ALL_ACCESS);

			GUARDIAN_AUTO_THROW(this->FenceObject->SetEventOnCompletion(this->FenceValue, EventHandle));

			WaitForSingleObject(EventHandle, INFINITE);
			CloseHandle(EventHandle);
		}
	}

	WRL::ComPtr<ID3D12Fence> GFence::GetFenceObject()
	{
		return this->FenceObject;
	}
}

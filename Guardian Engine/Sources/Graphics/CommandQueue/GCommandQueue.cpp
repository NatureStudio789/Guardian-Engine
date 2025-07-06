#include "GCommandQueue.h"

namespace GE
{
	GCommandQueue::GCommandQueue()
	{

	}

	GCommandQueue::GCommandQueue(std::shared_ptr<GDevice> device)
	{
		this->InitializeCommandQueue(device);
	}

	GCommandQueue::GCommandQueue(const GCommandQueue& other)
	{
		this->CommandQueueObject = other.CommandQueueObject;
	}

	GCommandQueue::~GCommandQueue()
	{
	}

	void GCommandQueue::InitializeCommandQueue(std::shared_ptr<GDevice> device)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;
		GUARDIAN_CLEAR_MEMORY(CommandQueueDesc);
		CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		GUARDIAN_AUTO_THROW(
			device->GetDeviceObject()->CreateCommandQueue(&CommandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)this->CommandQueueObject.GetAddressOf()));
	}

	void GCommandQueue::ExecuteCommandLists(UINT commandListsCount, ID3D12CommandList* commandLists[])
	{
		this->CommandQueueObject->ExecuteCommandLists(commandListsCount, commandLists);
	}

	WRL::ComPtr<ID3D12CommandQueue> GCommandQueue::GetCommandQueueObject()
	{
		return this->CommandQueueObject;
	}
}

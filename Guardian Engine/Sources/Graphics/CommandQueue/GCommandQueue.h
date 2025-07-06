#ifndef _GE_GCOMMANDQUEUE_H_
#define _GE_GCOMMANDQUEUE_H_
#include "../Device/GDevice.h"

namespace GE
{
	class GUARDIAN_API GCommandQueue
	{
	public:
		GCommandQueue();
		GCommandQueue(std::shared_ptr<GDevice> device);
		GCommandQueue(const GCommandQueue& other);
		~GCommandQueue();

		void InitializeCommandQueue(std::shared_ptr<GDevice> device);

		void ExecuteCommandLists(UINT commandListsCount, ID3D12CommandList* commandLists[]);

		WRL::ComPtr<ID3D12CommandQueue> GetCommandQueueObject();

		static std::shared_ptr<GCommandQueue> CreateNewCommandQueue(std::shared_ptr<GDevice> device)
		{
			return std::make_shared<GCommandQueue>(device);
		}

	private:
		WRL::ComPtr<ID3D12CommandQueue> CommandQueueObject;
	};
}

#endif
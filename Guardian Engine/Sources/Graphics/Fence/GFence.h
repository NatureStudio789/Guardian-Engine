#ifndef _GE_GFENCE_H_
#define _GE_GFENCE_H_
#include "../CommandQueue/GCommandQueue.h"

namespace GE
{
	class GUARDIAN_API GFence
	{
	public:
		GFence();
		GFence(std::shared_ptr<GDevice> device);
		GFence(const GFence& other);
		~GFence();

		void InitializeFence(std::shared_ptr<GDevice> device);

		void FlushFence(std::shared_ptr<GCommandQueue> commandQueue);

		WRL::ComPtr<ID3D12Fence> GetFenceObject();

		static std::shared_ptr<GFence> CreateNewFence(std::shared_ptr<GDevice> device)
		{
			return std::make_shared<GFence>(device);
		}

	private:
		UINT FenceValue;
		WRL::ComPtr<ID3D12Fence> FenceObject;
	};
}

#endif
#ifndef _GE_GCOMMANDLIST_H_
#define _GE_GCOMMANDLIST_H_
#include "../Fence/GFence.h"

namespace GE
{
	class GUARDIAN_API GCommandList
	{
	public:
		GCommandList();
		GCommandList(std::shared_ptr<GDevice> device);
		GCommandList(const GCommandList& other);
		~GCommandList();

		void InitializeCommandList(std::shared_ptr<GDevice> device);

		WRL::ComPtr<ID3D12CommandAllocator> GetCommandListAllocator();
		WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandListObject();

		static std::shared_ptr<GCommandList> CreateNewCommandList(std::shared_ptr<GDevice> device)
		{
			return std::make_shared<GCommandList>(device);
		}

	private:
		WRL::ComPtr<ID3D12CommandAllocator> CommandListAllocator;
		WRL::ComPtr<ID3D12GraphicsCommandList> CommandListObject;
	};
}

#endif
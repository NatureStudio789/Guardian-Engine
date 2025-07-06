#ifndef _GE_GRENDERTARET_H_
#define _GE_GRENDERTARET_H_
#include "../DescriptorHeap/GDescriptorHeap.h"

namespace GE
{
	class GUARDIAN_API GRenderTarget
	{
	public:
		GRenderTarget();
		GRenderTarget(std::shared_ptr<GSwapChain> swapChain, 
			std::shared_ptr<GDevice> device, std::shared_ptr<GDescriptorHeap> rtvDescriptorHeap);
		GRenderTarget(const GRenderTarget& other);
		~GRenderTarget();

		void InitializeRenderTarget(std::shared_ptr<GSwapChain> swapChain, 
			std::shared_ptr<GDevice> device, std::shared_ptr<GDescriptorHeap> rtvDescriptorHeap);

		void ApplyRenderTarget(std::shared_ptr<GCommandList> commandList);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView();

		static std::shared_ptr<GRenderTarget> CreateNewRenderTarget(std::shared_ptr<GSwapChain> swapChain,
			std::shared_ptr<GDevice> device, std::shared_ptr<GDescriptorHeap> rtvDescriptorHeap)
		{
			return std::make_shared<GRenderTarget>(swapChain, device, rtvDescriptorHeap);
		}

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView;
	};
}

#endif
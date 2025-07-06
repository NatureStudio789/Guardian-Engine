#ifndef _GE_GSWAPCHAIN_H_
#define _GE_GSWAPCHAIN_H_
#include "../CommandList/GCommandList.h"

namespace GE
{
	class GUARDIAN_API GSwapChain
	{
	public:
		GSwapChain();
		GSwapChain(std::shared_ptr<GGraphicsFactory> factory,
			std::shared_ptr<GCommandQueue> commandQueue, 
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen);
		GSwapChain(const GSwapChain& other);
		~GSwapChain();

		void InitializeSwapChain(std::shared_ptr<GGraphicsFactory> factory,
			std::shared_ptr<GCommandQueue> commandQueue,
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen);

		void Present(UINT syncInternal);

		WRL::ComPtr<ID3D12Resource> GetBackBuffer();
		WRL::ComPtr<IDXGISwapChain> GetSwapChainObject();

		static std::shared_ptr<GSwapChain> CreateNewSwapChain(
			std::shared_ptr<GGraphicsFactory> factory,
			std::shared_ptr<GCommandQueue> commandQueue, HWND windowHandle,
			int bufferWidth, int bufferHeight, bool fullscreen)
		{
			return std::make_shared<GSwapChain>(factory, commandQueue, windowHandle, bufferWidth, bufferHeight, fullscreen);
		}

	private:
		WRL::ComPtr<IDXGISwapChain> SwapChainObject;
	};
}

#endif
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
			std::shared_ptr<GCommandQueue> commandQueue, UINT bufferCount, 
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen);
		GSwapChain(const GSwapChain& other);
		~GSwapChain();

		void InitializeSwapChain(std::shared_ptr<GGraphicsFactory> factory,
			std::shared_ptr<GCommandQueue> commandQueue, UINT bufferCount,
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen);
		void CreateBuffers();
		void ReleaseBuffers();

		void ResizeBuffer(int newWidth, int newHeight);
		void Present(UINT syncInternal);

		const int& GetBufferWidth() const noexcept;
		const int& GetBufferHeight() const noexcept;
		const UINT& GetBufferCount() const noexcept;
		const UINT& GetCurrentBufferIndex() const noexcept;
		WRL::ComPtr<ID3D12Resource> GetCurrentBuffer() const noexcept;
		std::vector<WRL::ComPtr<ID3D12Resource>> GetBufferList();
		WRL::ComPtr<IDXGISwapChain> GetSwapChainObject();

		static std::shared_ptr<GSwapChain> CreateNewSwapChain(
			std::shared_ptr<GGraphicsFactory> factory,
			std::shared_ptr<GCommandQueue> commandQueue, UINT bufferCount, 
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
		{
			return std::make_shared<GSwapChain>(factory, commandQueue, bufferCount, windowHandle, bufferWidth, bufferHeight, fullscreen);
		}

	private:
		int BufferWidth, BufferHeight;
		UINT BufferCount;
		UINT CurrentBufferIndex;
		std::vector<WRL::ComPtr<ID3D12Resource>> BufferList;

		WRL::ComPtr<IDXGISwapChain> SwapChainObject;
	};
}

#endif
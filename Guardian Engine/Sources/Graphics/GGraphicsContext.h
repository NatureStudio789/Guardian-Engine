#ifndef _GE_GGRAPHICSCONTEXT_H_
#define _GE_GGRAPHICSCONTEXT_H_
#include "DescriptorHeap/GDescriptorHeap.h"

namespace GE
{
	class GUARDIAN_API GGraphicsContext
	{
	public:
		GGraphicsContext();
		GGraphicsContext(HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen);
		GGraphicsContext(const GGraphicsContext& other);
		~GGraphicsContext();

		void InitializeGraphicsContext(HWND windowHandle,
			int bufferWidth, int bufferHeight, bool fullscreen);

		void BeginRendering();
		void EndUpRendering(UINT syncInternal);

		const GUUID& GetContextId() const noexcept;

		static std::shared_ptr<GGraphicsContext> CreateNewGraphicsContext(
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
		{
			return std::make_shared<GGraphicsContext>(windowHandle, bufferWidth, bufferHeight, fullscreen);
		}

	private:
		GUUID ContextId = GUUID();

		std::shared_ptr<GGraphicsFactory> GraphicsFactory;
		std::shared_ptr<GDevice> GraphicsDevice;

		std::shared_ptr<GCommandQueue> GraphicsCommandQueue;
		std::shared_ptr<GCommandList> GraphicsCommandList;

		std::shared_ptr<GSwapChain> GraphicsSwapChain;
		WRL::ComPtr<ID3D12Fence> GraphicsFence;

		std::shared_ptr<GDescriptorHeap> RTVDescriptorHeap;
		int RTVDescriptorSize;
		std::shared_ptr<GDescriptorHeap> DSVDescriptorHeap;
		int DSVDescriptorSize;
	};
}

#endif
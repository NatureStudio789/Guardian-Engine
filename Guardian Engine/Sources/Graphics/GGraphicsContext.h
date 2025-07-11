#ifndef _GE_GGRAPHICSCONTEXT_H_
#define _GE_GGRAPHICSCONTEXT_H_
#include "RootSignature/GRootSignature.h"

namespace GE
{
	class GUARDIAN_API GFramebuffer;

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
		void ApplyMainFramebuffer();
		void EndUpRendering(UINT syncInternal);

		const GUUID& GetContextId() const noexcept;
		std::shared_ptr<GGraphicsFactory> GetGraphicsFactory();
		std::shared_ptr<GDevice> GetGraphicsDevice();
		std::shared_ptr<GFence> GetGraphicsFence();

		std::shared_ptr<GCommandQueue> GetGraphicsCommandQueue();
		std::shared_ptr<GCommandList> GetGraphicsCommandList();

		std::shared_ptr<GSwapChain> GetGraphicsSwapChain();
		std::shared_ptr<GRootSignature> GetGraphicsRootSignature();

		static std::shared_ptr<GGraphicsContext> CreateNewGraphicsContext(
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
		{
			return std::make_shared<GGraphicsContext>(windowHandle, bufferWidth, bufferHeight, fullscreen);
		}

	private:
		GUUID ContextId = GUUID();
		HWND ContextWindowHandle;
		std::shared_ptr<GEventProcesser> ContextEventProcesser;

		std::shared_ptr<GGraphicsFactory> GraphicsFactory;
		std::shared_ptr<GDevice> GraphicsDevice;
		std::shared_ptr<GFence> GraphicsFence;

		std::shared_ptr<GCommandQueue> GraphicsCommandQueue;
		std::shared_ptr<GCommandList> GraphicsCommandList;

		std::shared_ptr<GSwapChain> GraphicsSwapChain;
		std::shared_ptr<GRootSignature> GraphicsRootSignature;

		std::shared_ptr<GFramebuffer> GraphicsMainFramebuffer;
	};
}

#endif
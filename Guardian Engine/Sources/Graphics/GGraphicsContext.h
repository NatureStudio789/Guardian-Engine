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
		GGraphicsContext(HWND windowHandle,
			int bufferWidth, int bufferHeight, bool fullscreen);
		GGraphicsContext(const GGraphicsContext& other);
		~GGraphicsContext();

		void InitializeGraphicsContext(HWND windowHandle,
			int bufferWidth, int bufferHeight, bool fullscreen);

		void RegisterGraphicsCommandList(const std::string& name);
		void SetCurrentCommandList(const std::string& name);

		void BeginRendering();
		void ApplyDescriptorHeaps();
		void EndUpRendering();
		void PresentRenderingResult(UINT syncInternal);

		void ExecuteInitialization();

		void ExecuteCommandList();
		void ExecuteCommandListBatch();
		
		void FlushCommandQueue();

		const GUUID& GetContextId() const noexcept;
		HWND GetContextWindowHandle() noexcept;
		std::shared_ptr<GGraphicsFactory> GetGraphicsFactory();
		std::shared_ptr<GDevice> GetGraphicsDevice();
		std::shared_ptr<GFence> GetGraphicsFence();

		std::shared_ptr<GCommandQueue> GetGraphicsCommandQueue();
		std::shared_ptr<GCommandList> GetInitializationCommandList();
		std::shared_ptr<GCommandList> GetGraphicsCommandList();
		std::map<std::string, std::shared_ptr<GCommandList>> GetGraphicsCommandListBatch();

		std::shared_ptr<GSwapChain> GetGraphicsSwapChain();

		std::shared_ptr<GDescriptorHeap> GetRTVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetDSVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetSRVDescriptorHeap();
		std::shared_ptr<GDescriptorHeap> GetSamplerDescriptorHeap();

		static std::shared_ptr<GGraphicsContext> CreateNewGraphicsContext(
			HWND windowHandle, int bufferWidth, int bufferHeight, bool fullscreen)
		{
			return std::make_shared<GGraphicsContext>(windowHandle, bufferWidth, bufferHeight, fullscreen);
		}

	private:
		GUUID ContextId = GUUID();
		HWND ContextWindowHandle;

		std::shared_ptr<GGraphicsFactory> GraphicsFactory;
		std::shared_ptr<GDevice> GraphicsDevice;
		std::shared_ptr<GFence> GraphicsFence;

		std::shared_ptr<GCommandQueue> GraphicsCommandQueue;
		std::string CurrentCommandListName;
		std::map<std::string, std::shared_ptr<GCommandList>> GraphicsCommandListBatch;

		std::shared_ptr<GSwapChain> GraphicsSwapChain;

		std::shared_ptr<GDescriptorHeap> RTVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> DSVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> SRVDescriptorHeap;
		std::shared_ptr<GDescriptorHeap> SamplerDescriptorHeap;
	};
}

#endif
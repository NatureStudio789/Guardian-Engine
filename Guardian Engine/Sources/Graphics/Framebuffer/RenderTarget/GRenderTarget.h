#ifndef _GE_GRENDERTARET_H_
#define _GE_GRENDERTARET_H_
#include "../../GGraphicsContext.h"

namespace GE
{
	class GUARDIAN_API GRenderTarget
	{
	public:
		GRenderTarget();
		GRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext);
		GRenderTarget(const GRenderTarget& other);
		~GRenderTarget();

		void InitializeRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext);

		void ResizeRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ClearRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ApplyRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext,
			D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(std::shared_ptr<GGraphicsContext> graphicsContext);
		std::shared_ptr<GDescriptorHeap> GetRenderTargetViewDescriptorHeap();

		static std::shared_ptr<GRenderTarget> CreateNewRenderTarget(std::shared_ptr<GGraphicsContext> graphicsContext)
		{
			return std::make_shared<GRenderTarget>(graphicsContext);
		}

	private:
		std::shared_ptr<GDescriptorHeap> RenderTargetViewDescriptorHeap;
	};
}

#endif
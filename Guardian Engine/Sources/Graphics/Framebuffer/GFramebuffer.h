#ifndef _GE_GFRAMEBUFFER_H_
#define _GE_GFRAMEBUFFER_H_
#include "Viewport/GViewport.h"

namespace GE
{
	class GUARDIAN_API GFramebuffer
	{
	public:
		GFramebuffer();
		GFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer();

		void InitializeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext);

		void ApplyFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ResizeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, int newWidth, int newHeight);

		const GUUID& GetFramebufferId() const noexcept;
		std::shared_ptr<GRenderTarget> GetFramebufferRenderTarget();
		std::shared_ptr<GDepthStencil> GetFramebufferDepthStencil();
		std::shared_ptr<GViewport> GetFramebufferViewport();

		static std::shared_ptr<GFramebuffer> CreateNewFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext)
		{
			return std::make_shared<GFramebuffer>(graphicsContext);
		}

	private:
		GUUID FramebufferId = GUUID();

		int FramebufferWidth;
		int FramebufferHeight;

		std::shared_ptr<GRenderTarget> FramebufferRenderTarget;
		std::shared_ptr<GDepthStencil> FramebufferDepthStencil;
		std::shared_ptr<GViewport> FramebufferViewport;
	};
}

#endif
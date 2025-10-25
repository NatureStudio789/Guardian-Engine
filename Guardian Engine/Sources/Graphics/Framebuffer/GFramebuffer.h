#ifndef _GE_GFRAMEBUFFER_H_
#define _GE_GFRAMEBUFFER_H_
#include "Viewport/GViewport.h"

namespace GE
{
	class GUARDIAN_API GFramebuffer : public GShaderView
	{
	public:
		GFramebuffer();
		GFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false);
		GFramebuffer(const GFramebuffer& other);
		~GFramebuffer() override;

		void InitializeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false);

		void ApplyFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ClearFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext);
		void ResizeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, int newWidth, int newHeight);

		void BeginRendering(std::shared_ptr<GGraphicsContext> graphicsContext);
		void EndUpRendering(std::shared_ptr<GGraphicsContext> graphicsContext);

		const bool& IsEnableRTT() const noexcept;
		const GUUID& GetFramebufferId() const noexcept;
		const int& GetFramebufferWidth() const noexcept;
		const int& GetFramebufferHeight() const noexcept;
		std::shared_ptr<GRenderTarget> GetFramebufferRenderTarget();
		std::shared_ptr<GDepthStencil> GetFramebufferDepthStencil();
		std::shared_ptr<GViewport> GetFramebufferViewport();

		static std::shared_ptr<GFramebuffer> CreateNewFramebuffer(
			std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT = false)
		{
			return std::make_shared<GFramebuffer>(graphicsContext, enableRTT);
		}

	private:
		GUUID FramebufferId = GUUID();
		bool EnableRTT;

		int FramebufferWidth;
		int FramebufferHeight;

		std::shared_ptr<GRenderTarget> FramebufferRenderTarget;
		std::shared_ptr<GDepthStencil> FramebufferDepthStencil;
		std::shared_ptr<GViewport> FramebufferViewport;
	};
}

#endif
#ifndef _GE_GUARDIANFRAMEBUFFER_H_
#define _GE_GUARDIANFRAMEBUFFER_H_
#include "../RenderTarget/GuardianRenderTarget.h"

namespace GE
{
	enum GUARDIAN_API GuardianFramebufferCategory
	{
		GE_DEFAULT_FRAMEBUFFER,
		GE_SHADERRESOURCE_FRAMEBUFFER
	};

	class GUARDIAN_API GuardianFramebuffer
	{
	public:
		GuardianFramebuffer();
		GuardianFramebuffer(std::shared_ptr<GuardianGraphics> graphics);
		GuardianFramebuffer(
			std::shared_ptr<GuardianGraphics> graphics, int width, int height);
		GuardianFramebuffer(const GuardianFramebuffer& other);
		~GuardianFramebuffer();

		void InitializeFramebuffer(std::shared_ptr<GuardianGraphics> graphics);
		void InitializeFramebuffer(
			std::shared_ptr<GuardianGraphics> graphics, int width, int height);

		void ApplyFramebuffer(
			std::shared_ptr<GuardianGraphics> graphics, const GVector3& clearColor);
		
		void ResizeFramebuffer(
			std::shared_ptr<GuardianGraphics> graphics, int resizeWidth, int resizeHeight);

		const GuardianFramebufferCategory& GetFramebufferCategory() const noexcept;
		const int GetFramebufferWidth() const;
		const int GetFramebufferHeight() const;

		WRL::ComPtr<ID3D11ShaderResourceView> GetFramebufferResource();
		WRL::ComPtr<ID3D11ShaderResourceView> GetDepthStencilResource();
		std::shared_ptr<GuardianRenderTarget> GetFramebufferRenderTarget() noexcept;
		std::shared_ptr<GuardianDepthStencil> GetFramebufferDepthStencil() noexcept;
		std::shared_ptr<GuardianViewport> GetFramebufferViewport() noexcept;

	private:
		GuardianFramebufferCategory FramebufferCategory;
		int FramebufferWidth;
		int FramebufferHeight;

		std::shared_ptr<GuardianRenderTarget> RenderTarget;
		std::shared_ptr<GuardianDepthStencil> DepthStencil;
		std::shared_ptr<GuardianViewport> Viewport;
	};
}

#endif
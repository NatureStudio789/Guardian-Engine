#include "GFramebuffer.h"

namespace GE
{
	GFramebuffer::GFramebuffer()
	{
		this->FramebufferId = GUUID();
		this->FramebufferWidth = this->FramebufferHeight = 0;

		this->FramebufferRenderTarget = null;
		this->FramebufferDepthStencil = null;
		this->FramebufferViewport = null;
	}

	GFramebuffer::GFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT)
	{
		this->InitializeFramebuffer(graphicsContext, enableRTT);
	}

	GFramebuffer::GFramebuffer(const GFramebuffer& other)
	{
		this->FramebufferId = other.FramebufferId;

		this->FramebufferWidth = other.FramebufferWidth;
		this->FramebufferHeight = other.FramebufferHeight;

		this->FramebufferRenderTarget = other.FramebufferRenderTarget;
		this->FramebufferDepthStencil = other.FramebufferDepthStencil;
		this->FramebufferViewport = other.FramebufferViewport;
	}

	GFramebuffer::~GFramebuffer()
	{
		this->FramebufferId = 0;
		this->FramebufferWidth = this->FramebufferHeight = 0;

		this->FramebufferRenderTarget.reset();
		this->FramebufferRenderTarget = null;
		this->FramebufferDepthStencil.reset();
		this->FramebufferDepthStencil = null;
		this->FramebufferViewport.reset();
		this->FramebufferViewport = null;
	}

	void GFramebuffer::InitializeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, bool enableRTT)
	{
		this->EnableRTT = enableRTT;

		this->FramebufferWidth = graphicsContext->GetGraphicsSwapChain()->GetBufferWidth();
		this->FramebufferHeight = graphicsContext->GetGraphicsSwapChain()->GetBufferHeight();

		this->FramebufferRenderTarget = GRenderTarget::CreateNewRenderTarget(graphicsContext, this->EnableRTT);
		this->FramebufferDepthStencil = GDepthStencil::CreateNewDepthStencil(graphicsContext);

		GViewport::Attribute ViewportAttribute;
		GUARDIAN_CLEAR_MEMORY(ViewportAttribute);
		ViewportAttribute.PositionX = 0.0f;
		ViewportAttribute.PositionY = 0.0f;
		ViewportAttribute.Width = (float)this->FramebufferWidth;
		ViewportAttribute.Height = (float)this->FramebufferHeight;
		ViewportAttribute.MinDepth = 0.0f;
		ViewportAttribute.MaxDepth = 1.0f;
		this->FramebufferViewport = GViewport::CreateNewViewport(ViewportAttribute);
	}

	void GFramebuffer::ApplyFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext)
	{
		this->FramebufferViewport->ApplyViewport(graphicsContext->GetGraphicsCommandList());

		this->FramebufferRenderTarget->ClearRenderTarget(graphicsContext);
		this->FramebufferDepthStencil->ClearDepthStencil(graphicsContext);

		this->FramebufferRenderTarget->ApplyRenderTarget(graphicsContext, 
			this->FramebufferDepthStencil->GetDSVDescriptorHandle()->CPUHandle);
	}

	void GFramebuffer::ResizeFramebuffer(std::shared_ptr<GGraphicsContext> graphicsContext, int newWidth, int newHeight)
	{
		GUARDIAN_CHECK_VALUE(newWidth);
		GUARDIAN_CHECK_VALUE(newHeight);

		this->FramebufferRenderTarget->ResizeRenderTargetView(graphicsContext, newWidth, newHeight);
		this->FramebufferDepthStencil->ResizeDepthStencilView(graphicsContext);

		GViewport::Attribute ViewportAttribute;
		GUARDIAN_CLEAR_MEMORY(ViewportAttribute);
		ViewportAttribute.PositionX = 0.0f;
		ViewportAttribute.PositionY = 0.0f;
		ViewportAttribute.Width = (float)newWidth;
		ViewportAttribute.Height = (float)newHeight;
		ViewportAttribute.MinDepth = 0.0f;
		ViewportAttribute.MaxDepth = 1.0f;
		this->FramebufferViewport->SetViewportAttribute(ViewportAttribute);
	}

	const bool& GFramebuffer::IsEnableRTT() const noexcept
	{
		return this->EnableRTT;
	}

	const GUUID& GFramebuffer::GetFramebufferId() const noexcept
	{
		return this->FramebufferId;
	}

	const int& GFramebuffer::GetFramebufferWidth() const noexcept
	{
		return this->FramebufferWidth;
	}

	const int& GFramebuffer::GetFramebufferHeight() const noexcept
	{
		return this->FramebufferHeight;
	}

	std::shared_ptr<GRenderTarget> GFramebuffer::GetFramebufferRenderTarget()
	{
		return this->FramebufferRenderTarget;
	}

	std::shared_ptr<GDepthStencil> GFramebuffer::GetFramebufferDepthStencil()
	{
		return this->FramebufferDepthStencil;
	}

	std::shared_ptr<GViewport> GFramebuffer::GetFramebufferViewport()
	{
		return this->FramebufferViewport;
	}
}

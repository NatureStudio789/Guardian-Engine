#include "GuardianFramebuffer.h"
#include "../GuardianGraphics.h"

namespace GE
{
	GuardianFramebuffer::GuardianFramebuffer()
	{
		this->FramebufferCategory = GE_DEFAULT_FRAMEBUFFER;
		this->FramebufferWidth = 0;
		this->FramebufferHeight = 0;

		this->RenderTarget = std::make_shared<GuardianRenderTarget>();
		this->DepthStencil = std::make_shared<GuardianDepthStencil>();
		this->Viewport = std::make_shared<GuardianViewport>();
	}

	GuardianFramebuffer::GuardianFramebuffer(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->RenderTarget = std::make_shared<GuardianRenderTarget>();
		this->DepthStencil = std::make_shared<GuardianDepthStencil>();
		this->Viewport = std::make_shared<GuardianViewport>();

		this->InitializeFramebuffer(graphics);
	}

	GuardianFramebuffer::GuardianFramebuffer(std::shared_ptr<GuardianGraphics> graphics, int width, int height, bool isDepthGraph)
	{
		this->RenderTarget = std::make_shared<GuardianRenderTarget>();
		this->DepthStencil = std::make_shared<GuardianDepthStencil>();
		this->Viewport = std::make_shared<GuardianViewport>();

		this->InitializeFramebuffer(graphics, width, height, isDepthGraph);
	}

	GuardianFramebuffer::GuardianFramebuffer(const GuardianFramebuffer& other)
	{
		this->FramebufferCategory = other.FramebufferCategory;
		this->FramebufferWidth = other.FramebufferWidth;
		this->FramebufferHeight = other.FramebufferHeight;
		this->RenderTarget = other.RenderTarget;
		this->DepthStencil = other.DepthStencil;
		this->Viewport = other.Viewport;
	}

	GuardianFramebuffer::~GuardianFramebuffer()
	{
	}

	void GuardianFramebuffer::InitializeFramebuffer(std::shared_ptr<GuardianGraphics> graphics)
	{
		this->FramebufferCategory = GE_DEFAULT_FRAMEBUFFER;
		DXGI_SWAP_CHAIN_DESC SwapChainDesc;
		graphics->GetGraphicsSwapChain()->GetDesc(&SwapChainDesc);
		this->FramebufferWidth = SwapChainDesc.BufferDesc.Width;
		this->FramebufferHeight = SwapChainDesc.BufferDesc.Height;

		this->RenderTarget->InitializeRenderTarget(graphics);

		this->DepthStencil->InitializeDepthStencil(graphics, this->FramebufferWidth, this->FramebufferHeight);
		this->Viewport->InitializeViewport(GVector2(0.0f, 0.0f),
			GVector2((float)this->FramebufferWidth, (float)this->FramebufferHeight));
	}

	void GuardianFramebuffer::InitializeFramebuffer(std::shared_ptr<GuardianGraphics> graphics, int width, int height, bool isDepthGraph)
	{
		this->FramebufferCategory = GE_SHADERRESOURCE_FRAMEBUFFER;
		this->FramebufferWidth = width;
		this->FramebufferHeight = height;

		this->RenderTarget->InitializeRenderTarget(graphics, this->FramebufferWidth, this->FramebufferHeight);

		this->DepthStencil->InitializeDepthStencil(graphics, this->FramebufferWidth, this->FramebufferHeight, isDepthGraph);
		this->Viewport->InitializeViewport(GVector2(0.0f, 0.0f),
			GVector2((float)this->FramebufferWidth, (float)this->FramebufferHeight));
	}

	void GuardianFramebuffer::ApplyFramebuffer(
		std::shared_ptr<GuardianGraphics> graphics, const GVector3& clearColor)
	{
		this->RenderTarget->ApplyRenderTarget(graphics, this->DepthStencil, clearColor);
		this->DepthStencil->ClearDepthStencil(graphics);

		this->Viewport->ApplyViewport(graphics);
	}

	void GuardianFramebuffer::ResizeFramebuffer(
		std::shared_ptr<GuardianGraphics> graphics, int resizeWidth, int resizeHeight)
	{
		this->RenderTarget->GetRenderTargetView()->Release();
		this->RenderTarget->GetRenderTargetTexture()->Release();
		this->DepthStencil->GetDepthStencilView()->Release();
		this->DepthStencil->GetDepthStencilState()->Release();
		this->DepthStencil->GetDepthStencilShaderResource()->Release();

		if (this->FramebufferCategory == GE_DEFAULT_FRAMEBUFFER)
		{
			this->InitializeFramebuffer(graphics);
		}
		else if (this->FramebufferCategory == GE_SHADERRESOURCE_FRAMEBUFFER)
		{
			this->RenderTarget->GetRenderTargetShaderResource()->Release();

			this->InitializeFramebuffer(graphics, resizeWidth, resizeHeight);
		}
	}

	const GuardianFramebufferCategory& GuardianFramebuffer::GetFramebufferCategory() const noexcept
	{
		return this->FramebufferCategory;
	}

	const int GuardianFramebuffer::GetFramebufferWidth() const
	{
		return this->FramebufferWidth;
	}

	const int GuardianFramebuffer::GetFramebufferHeight() const
	{
		return this->FramebufferHeight;
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianFramebuffer::GetFramebufferResource()
	{
		if (this->FramebufferCategory != GE_SHADERRESOURCE_FRAMEBUFFER)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The frame buffer doesn't support to get the resource!");
		}

		return this->RenderTarget->GetRenderTargetShaderResource();
	}

	WRL::ComPtr<ID3D11ShaderResourceView> GuardianFramebuffer::GetDepthStencilResource()
	{
		return this->DepthStencil->GetDepthStencilShaderResource();
	}

	std::shared_ptr<GuardianRenderTarget> GuardianFramebuffer::GetFramebufferRenderTarget() noexcept
	{
		return this->RenderTarget;
	}

	std::shared_ptr<GuardianDepthStencil> GuardianFramebuffer::GetFramebufferDepthStencil() noexcept
	{
		return this->DepthStencil;
	}

	std::shared_ptr<GuardianViewport> GuardianFramebuffer::GetFramebufferViewport() noexcept
	{
		return this->Viewport;
	}
}

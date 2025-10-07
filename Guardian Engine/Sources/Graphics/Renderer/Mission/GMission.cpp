#include "GMission.h"
#include "../GRenderer.h"

namespace GE
{
	GMission::GMission()
	{
		this->TargetGraphName = "";
		this->RenderFramebuffer = null;
		this->RenderCamera = null;
	}

	GMission::GMission(const std::string& targetGraphName, std::shared_ptr<GFramebuffer> framebuffer, std::shared_ptr<GCamera> camera)
	{
		this->SetTargetGraphName(targetGraphName);
		this->SetRenderFramebuffer(framebuffer);
		this->SetRenderCamera(camera);
	}

	GMission::GMission(const GMission& other)
	{
		this->TargetGraphName = other.TargetGraphName;
		this->RenderFramebuffer = other.RenderFramebuffer;
		this->RenderCamera = other.RenderCamera;
	}

	GMission::~GMission()
	{
		this->TargetGraphName.clear();

		this->RenderFramebuffer = null;
		this->RenderCamera = null;
	}

	void GMission::SetTargetGraphName(const std::string& targetGraphName)
	{
		this->TargetGraphName = targetGraphName;
	}

	void GMission::SetRenderFramebuffer(std::shared_ptr<GFramebuffer> framebuffer)
	{
		this->RenderFramebuffer = framebuffer;
	}

	void GMission::SetRenderCamera(std::shared_ptr<GCamera> camera)
	{
		this->RenderCamera = camera;
	}

	void GMission::Request()
	{
		GRenderer::Accept(this);
	}

	const std::string& GMission::GetTargetRenderGraphName() const noexcept
	{
		return this->TargetGraphName;
	}

	std::shared_ptr<GFramebuffer> GMission::GetRenderFramebuffer() const noexcept
	{
		return this->RenderFramebuffer;
	}

	std::shared_ptr<GCamera> GMission::GetRenderCamera() const noexcept
	{
		return this->RenderCamera;
	}
}

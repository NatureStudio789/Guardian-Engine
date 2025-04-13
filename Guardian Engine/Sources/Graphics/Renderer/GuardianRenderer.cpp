#include "GuardianRenderer.h"
#include "../../Application/GuardianApplication.h"

namespace GE
{
	std::map<GString, std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>>> GuardianRenderer::RenderableList;
	std::map<GString, std::shared_ptr<GuardianFramebuffer>> GuardianRenderer::RenderingFramebufferList;
	std::map<GString, GuardianCamera> GuardianRenderer::RenderingCameraList;

	GVector3 GuardianRenderer::RenderingClearColor;


	void GuardianRenderer::CreateRenderingFramebuffer(const GString& framebufferName,
		const GuardianCamera& camera, int width, int height)
	{
		if (RenderingFramebufferList.count(framebufferName) <= 0)
		{
			RenderingFramebufferList[framebufferName] = std::make_shared<GuardianFramebuffer>(
				GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), width, height);
			RenderingCameraList[framebufferName] = camera;
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer already has had this framebuffer!");
		}
	}

	void GuardianRenderer::ResizeRenderingFramebuffer(const GString& framebufferName, int width, int height)
	{
		if (RenderingFramebufferList.count(framebufferName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No framebuffers called : '" + framebufferName + "' found in the renderer!");
		}
		else
		{
			RenderingFramebufferList[framebufferName]->ResizeFramebuffer(
				GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), width, height);
		}
	}

	void GuardianRenderer::SetFramebufferCamera(const GString& framebufferName, const GuardianCamera& camera)
	{
		if (RenderingFramebufferList.count(framebufferName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No framebuffers called : '" + framebufferName + "' found in the renderer!");
		}
		else
		{
			RenderingCameraList[framebufferName] = camera;
		}
	}

	std::shared_ptr<GuardianFramebuffer> GuardianRenderer::GetRenderingFramebuffer(const GString& name)
	{
		if (RenderingFramebufferList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No framebuffers called : '" + name + "' found in the renderer!");
		}
		else
		{
			return RenderingFramebufferList[name];
		}
	}

	void GuardianRenderer::SubmitRenderable(GuardianSubmitPassLevel level,
		const GString& submitFramebuffer, std::shared_ptr<GuardianRenderable> renderable)
	{
		if (RenderingFramebufferList.count(submitFramebuffer) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer has no framebuffers called : '" + submitFramebuffer + "' !");
		}

		RenderableList[submitFramebuffer][level].push(renderable);
	}

	void GuardianRenderer::SetClearColor(const GVector3& color)
	{
		RenderingClearColor = color;
	}

	void GuardianRenderer::Render()
	{
		for (auto& framebuffer : RenderingFramebufferList)
		{
			framebuffer.second->ApplyFramebuffer(
				GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), RenderingClearColor);

			for (UINT i = 0; i <= (UINT)GE_SUBMIT_SPECIALLY; i++)
			{
				while (!RenderableList[framebuffer.first][(GuardianSubmitPassLevel)i].empty())
				{
					auto& renderable = RenderableList[framebuffer.first][(GuardianSubmitPassLevel)i].front();

					renderable->GetTransformConstantBuffer()->UpdateData(
						GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
						{ renderable->GetTransformConstantBuffer()->GetConstantBufferData().WorldTransformMatrix,
						RenderingCameraList[framebuffer.first].GetViewMatrix(), RenderingCameraList[framebuffer.first].GetProjectionMatrix() });
					renderable->Update();
					
					renderable->Render(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext());

					RenderableList[framebuffer.first][(GuardianSubmitPassLevel)i].pop();
				}
			}
		}
	}
}

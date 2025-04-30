#include "GuardianRenderer.h"

namespace GE
{
	std::map<GString, std::shared_ptr<GuardianRenderGraph>> GuardianRenderer::RenderingRenderGraphList;

	void GuardianRenderer::CreateRenderingRenderGraph(const GString& renderGraphName, int width, int height)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			RenderingRenderGraphList[renderGraphName] = std::make_shared<GuardianRenderGraph>(renderGraphName, width, height);
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer already has had this render graph!");
		}
	}

	void GuardianRenderer::ResizeRenderingRenderGraph(const GString& renderGraphName, int width, int height)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + renderGraphName + "' found in the renderer!");
		}
		else
		{
			RenderingRenderGraphList[renderGraphName]->ResizeRenderGraph(width, height);
		}
	}

	void GuardianRenderer::SetRenderingRenderGraphShaderGroup(const GString& renderGraphName, const GString& shaderGroupName)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + renderGraphName + "' found in the renderer!");
		}
		else
		{
			RenderingRenderGraphList[renderGraphName]->SetGraphShaderGroup(shaderGroupName);
		}
	}

	void GuardianRenderer::SetRenderingRenderGraphCamera(const GString& renderGraphName, const GuardianCamera& camera)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + renderGraphName + "' found in the renderer!");
		}
		else
		{
			RenderingRenderGraphList[renderGraphName]->SetGraphCamera(camera);
		}
	}

	std::shared_ptr<GuardianFramebuffer> GuardianRenderer::GetRenderingRenderGraphFramebuffer(const GString& name)
	{
		if (RenderingRenderGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + name + "' found in the renderer!");
		}
		else
		{
			return RenderingRenderGraphList[name]->GetRenderGraphFramebuffer();
		}
	}

	void GuardianRenderer::SetRenderingRenderGraphClearColor(const GString& renderGraphName, const GVector3& color)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer has no render graph called : '" + renderGraphName + "' !");
		}

		RenderingRenderGraphList[renderGraphName]->SetGraphClearColor(color);
	}

	void GuardianRenderer::SubmitRenderable(GuardianSubmitPassLevel level,
		const GString& renderGraphName, std::shared_ptr<GuardianRenderable> renderable)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer has no render graph called : '" + renderGraphName + "' !");
		}

		RenderingRenderGraphList[renderGraphName]->SubmitRenderable(level, renderable);
	}

	void GuardianRenderer::Render()
	{
		for (auto& renderGraph : RenderingRenderGraphList)
		{
			renderGraph.second->Render();
		}
	}
}

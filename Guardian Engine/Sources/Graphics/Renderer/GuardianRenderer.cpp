#include "GuardianRenderer.h"
#include "../../ECS/Scene/GuardianScene.h"

namespace GE
{
	std::map<GString, std::shared_ptr<GuardianRenderGraph>> GuardianRenderer::RenderingRenderGraphList;
	std::map<GString, std::shared_ptr<GuardianSceneGraph>> GuardianRenderer::RenderingSceneGraphList;


	void GuardianRenderer::InitializeRenderer()
	{
		GuardianDepthGraph::DepthMapSampler =
			std::make_shared<GuardianSampler>(GuardianSampler::Properties(GuardianSampler::GE_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
				GuardianSampler::GE_TEXTURE_ADDRESS_BORDER, GuardianSampler::GE_TEXTURE_ADDRESS_BORDER, GuardianSampler::GE_TEXTURE_ADDRESS_BORDER), 1);
	}

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

	void GuardianRenderer::CreateRenderingRenderGraphDepthGraph(
		const GString& renderGraphName, const GString& depthGraphName, int width, int height)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + renderGraphName + "' found in the renderer!");
		}
		else
		{
			RenderingRenderGraphList[renderGraphName]->AddDepthGraph(depthGraphName, width, height);
		}
	}

	std::shared_ptr<GuardianDepthGraph> GuardianRenderer::GetRenderingRenderGraphDepthGraph(const GString& renderGraphName, const GString& depthGraphName)
	{
		if (RenderingRenderGraphList.count(renderGraphName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + renderGraphName + "' found in the renderer!");
		}
		else
		{
			return RenderingRenderGraphList[renderGraphName]->GetDepthGraph(depthGraphName);
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

	void GuardianRenderer::CreateRenderingSceneGraph(GuardianScene* scene, int width, int height)
	{
		if (RenderingSceneGraphList.count(scene->SceneName) <= 0)
		{
			RenderingSceneGraphList[scene->SceneName] = std::make_shared<GuardianSceneGraph>(scene, width, height);
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer already has had this render graph!");
		}
	}

	std::shared_ptr<GuardianSceneGraph> GuardianRenderer::GetRenderingSceneGraph(GuardianScene* scene)
	{
		if (RenderingSceneGraphList.count(scene->SceneName) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("No render graph called : '" + scene->SceneName + "' found in the renderer!");
		}
		else
		{
			return RenderingSceneGraphList[scene->SceneName];
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
		const GString& graphName, std::shared_ptr<GuardianRenderable> renderable)
	{
		if (RenderingRenderGraphList.count(graphName) > 0)
		{
			RenderingRenderGraphList[graphName]->SubmitRenderable(level, renderable);
		}
		else if (RenderingSceneGraphList.count(graphName) > 0)
		{
			RenderingSceneGraphList[graphName]->SubmitRenderable(level, renderable);
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("The renderer has no graph called : '" + graphName + "' !");
		}
	}

	void GuardianRenderer::Render()
	{
		for (auto& sceneGraph : RenderingSceneGraphList)
		{
			sceneGraph.second->Render();
		}

		for (auto& renderGraph : RenderingRenderGraphList)
		{
			renderGraph.second->Render();
		}
	}
}

#include "GuardianRenderGraph.h"

namespace GE
{
	GuardianRenderGraph::GuardianRenderGraph()
	{
		this->GraphName = "";
		this->GraphClearColor = { 0.0f, 0.0f, 0.0f };

		this->GraphShaderGroup = null;

		this->GraphFramebuffer = null;
		this->GraphCamera = {};
		this->RenderableQueueList.clear();
		this->DepthGraphList.clear();
	}

	GuardianRenderGraph::GuardianRenderGraph(const GString& graphName, int width, int height)
	{
		this->InitializeRenderGraph(graphName, width, height);
	}

	GuardianRenderGraph::GuardianRenderGraph(const GuardianRenderGraph& other)
	{
		this->GraphName = other.GraphName;
		this->GraphClearColor = other.GraphClearColor;

		this->GraphShaderGroup = other.GraphShaderGroup;

		this->GraphFramebuffer = other.GraphFramebuffer;
		this->GraphCamera = other.GraphCamera;
		this->RenderableQueueList = other.RenderableQueueList;
		this->DepthGraphList = other.DepthGraphList;
	}

	GuardianRenderGraph::~GuardianRenderGraph()
	{
		this->GraphName.clear();
		this->GraphClearColor = { 0.0f, 0.0f, 0.0f };
		this->GraphShaderGroup = null;
		this->GraphFramebuffer.reset();
		this->GraphCamera = {};
		this->RenderableQueueList.clear();
		this->DepthGraphList.clear();
	}

	void GuardianRenderGraph::InitializeRenderGraph(const GString& graphName, int width, int height)
	{
		this->GraphName = graphName;

		this->GraphFramebuffer = std::make_shared<GuardianFramebuffer>(
			GuardianGraphicsRegistry::GetCurrentGraphics(), width, height);
	}

	void GuardianRenderGraph::SetGraphClearColor(const GVector3& color)
	{
		this->GraphClearColor = color;
	}

	void GuardianRenderGraph::SetGraphCamera(const GuardianCamera& camera)
	{
		this->GraphCamera = camera;
	}

	void GuardianRenderGraph::SetGraphShaderGroup(const GString& groupName)
	{
		this->GraphShaderGroup = GuardianShaderSystem::GetShaderGroup(groupName);
	}

	void GuardianRenderGraph::AddDepthGraph(const GString& name, int width, int height)
	{
		if (this->DepthGraphList.count(name) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("This render graph already has the depth graph named : '{}'!", name));
		}

		this->DepthGraphList[name] = std::make_shared<GuardianDepthGraph>(name, width, height);
	}

	void GuardianRenderGraph::SetDepthGraphCamera(const GString& name, const GuardianCamera& camera)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("This render graph hasn't the depth graph named : '{}'!", name));
		}

		this->DepthGraphList[name]->SetGraphCamera(camera);
	}

	std::shared_ptr<GuardianDepthGraph> GuardianRenderGraph::GetDepthGraph(const GString& name)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No depth graph found in this render graph named : '{}'!", name));
		}

		return this->DepthGraphList[name];
	}

	std::shared_ptr<GuardianDepthStencil> GuardianRenderGraph::GetDepthGraphDepthStencil(const GString& name)
	{
		if (this->DepthGraphList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No depth graph found in this render graph named : '{}'!", name));
		}

		return this->DepthGraphList[name]->GetGraphDepthStencil();
	}

	void GuardianRenderGraph::SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable)
	{
		this->RenderableQueueList[level].push(renderable);

		if (level != GE_SUBMIT_SPECIALLY && level != GE_SUBMIT_DEFAULT2D && 
			level != GE_SUBMIT_TRANSPARENT2D && level != GE_SUBMIT_GUI2D)
		{
			for (auto& depthGraph : this->DepthGraphList)
			{
				depthGraph.second->SubmitRenderable(renderable);
			}
		}
	}

	void GuardianRenderGraph::Render()
	{
		if (!this->GraphShaderGroup)
		{
			throw GUARDIAN_ERROR_EXCEPTION("This render graph hasn't been set shaders!");
		}

		this->GraphFramebuffer->ApplyFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), this->GraphClearColor);

		for (UINT i = 0; i <= (UINT)GE_SUBMIT_SPECIALLY; i++)
		{
			while (!this->RenderableQueueList[(GuardianSubmitPassLevel)i].empty())
			{
				auto& renderable = this->RenderableQueueList[(GuardianSubmitPassLevel)i].front();

				renderable->GetTransformConstantBuffer()->UpdateData(
					{ renderable->GetTransformConstantBuffer()->GetConstantBufferData().WorldTransformMatrix,
					this->GraphCamera.GetViewMatrix(), this->GraphCamera.GetProjectionMatrix() });
				renderable->Update();

				if (!renderable->UseSpecialShader)
				{
					this->GraphShaderGroup->Apply();
				}

				renderable->Render();

				this->RenderableQueueList[(GuardianSubmitPassLevel)i].pop();
			}
		}

		for (auto& depthGraph : this->DepthGraphList)
		{
			depthGraph.second->Render();
		}
	}

	void GuardianRenderGraph::ResizeRenderGraph(int width, int height)
	{
		this->GraphFramebuffer->ResizeFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), width, height);
	}

	std::shared_ptr<GuardianFramebuffer> GuardianRenderGraph::GetRenderGraphFramebuffer()
	{
		return this->GraphFramebuffer;
	}
}

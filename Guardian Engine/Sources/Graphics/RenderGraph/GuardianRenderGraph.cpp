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
	}

	GuardianRenderGraph::~GuardianRenderGraph()
	{
		this->GraphName.clear();
		this->GraphClearColor = { 0.0f, 0.0f, 0.0f };
		this->GraphShaderGroup = null;
		this->GraphFramebuffer.reset();
		this->GraphCamera = {};
		this->RenderableQueueList.clear();
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

	void GuardianRenderGraph::SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable)
	{
		this->RenderableQueueList[level].push(renderable);
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

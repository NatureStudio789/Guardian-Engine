#include "GuardianDepthGraph.h"

namespace GE
{
	std::shared_ptr<GuardianSampler> GuardianDepthGraph::DepthMapSampler;


	GuardianDepthGraph::GuardianDepthGraph()
	{
		this->GraphName = "";
		this->GraphCamera = {};
	}

	GuardianDepthGraph::GuardianDepthGraph(const GString& graphName, int width, int height)
	{
		this->InitializeDepthGraph(graphName, width, height);
	}

	GuardianDepthGraph::GuardianDepthGraph(const GuardianDepthGraph& other)
	{
		this->GraphName = other.GraphName;
		this->GraphFramebuffer = other.GraphFramebuffer;
		this->RenderableQueue = other.RenderableQueue;
		this->GraphCamera = other.GraphCamera;
	}

	GuardianDepthGraph::~GuardianDepthGraph()
	{
		this->GraphName.clear();
		this->GraphFramebuffer.reset();
		this->GraphCamera = {};
		
		while (!this->RenderableQueue.empty())
		{
			this->RenderableQueue.pop();
		}
	}

	void GuardianDepthGraph::InitializeDepthGraph(const GString& graphName, int width, int height)
	{
		this->GraphName = graphName;

		this->GraphFramebuffer = std::make_shared<GuardianFramebuffer>(
			GuardianGraphicsRegistry::GetCurrentGraphics(), width, height, true);
	}

	void GuardianDepthGraph::SetGraphCamera(const GuardianCamera& camera)
	{
		this->GraphCamera = camera;
	}

	void GuardianDepthGraph::SubmitRenderable(std::shared_ptr<GuardianRenderable> renderable)
	{
		this->RenderableQueue.push(renderable);
	}

	void GuardianDepthGraph::Render()
	{
		this->GraphFramebuffer->ApplyFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), { 0.0f, 0.0f, 0.0f });

		while (!this->RenderableQueue.empty())
		{
			auto& renderable = this->RenderableQueue.front();

			renderable->GetTransformConstantBuffer()->UpdateData(
				{ renderable->GetTransformConstantBuffer()->GetConstantBufferData().WorldTransformMatrix,
				this->GraphCamera.GetViewMatrix(), this->GraphCamera.GetProjectionMatrix() });
			renderable->Update();

			GuardianShaderSystem::GetShaderGroup(GuardianShaderSystem::SOLID_SHADER)->Apply();

			renderable->Render();

			this->RenderableQueue.pop();
		}
	}

	void GuardianDepthGraph::ResizeDepthGraph(int width, int height)
	{
		this->GraphFramebuffer->ResizeFramebuffer(GuardianGraphicsRegistry::GetCurrentGraphics(), width, height);
	}

	std::shared_ptr<GuardianFramebuffer> GuardianDepthGraph::GetGraphFramebuffer()
	{
		return this->GraphFramebuffer;
	}

	std::shared_ptr<GuardianDepthStencil> GuardianDepthGraph::GetGraphDepthStencil()
	{
		return this->GraphFramebuffer->GetFramebufferDepthStencil();
	}
}

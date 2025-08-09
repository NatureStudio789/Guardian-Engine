#include "GRenderQueuePass.h"

namespace GE
{
	void GRenderQueuePass::Accept(std::shared_ptr<GTask> task)
	{
		this->TaskList.push_back(task);
	}

	void GRenderQueuePass::Execute()
	{
		this->Framebuffer->BeginRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

		this->Apply();

		for (auto& task : this->TaskList)
		{
			task->Execute();
		}

		this->Framebuffer->EndUpRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
	}

	void GRenderQueuePass::Reset()
	{
		this->TaskList.clear();
	}
}

#include "GRenderQueuePass.h"

namespace GE
{
	void GRenderQueuePass::Accept(std::shared_ptr<GTask> task)
	{
		this->TaskList.push_back(task);
	}

	void GRenderQueuePass::Execute()
	{
		this->Apply();

		for (auto& task : this->TaskList)
		{
			task->Execute();
		}
	}

	void GRenderQueuePass::Reset()
	{
		this->TaskList.clear();
	}
}

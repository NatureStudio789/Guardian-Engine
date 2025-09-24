#include "GTask.h"

namespace GE
{
	GTask::GTask()
	{
		this->Step = null;
		this->Renderable = null;
	}

	GTask::GTask(std::shared_ptr<GStep> step, std::shared_ptr<GRenderable> renderable)
	{
		this->InitializeTask(step, renderable);
	}

	GTask::GTask(const GTask& other)
	{
		this->Step = other.Step;
		this->Renderable = other.Renderable;
	}

	GTask::~GTask()
	{

	}

	void GTask::InitializeTask(std::shared_ptr<GStep> step, std::shared_ptr<GRenderable> renderable)
	{
		this->Step = step;
		this->Renderable = renderable;
	}

	void GTask::Execute()
	{
		this->Renderable->Apply();
		this->Step->Apply();

		if (this->Renderable->HasIndexBuffer())
		{
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
				DrawIndexedInstanced(this->Renderable->GetIndicesCount(), 1, 0, 0, 0);
		}
		else
		{
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
				DrawInstanced(this->Renderable->GetVerticesCount(), 1, 0, 0);
		}
	}
}

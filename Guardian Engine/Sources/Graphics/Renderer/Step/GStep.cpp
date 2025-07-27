#include "GStep.h"
#include "../GRenderer.h"

namespace GE
{
	GStep::GStep()
	{
		this->ApplicableList.clear();

		this->TargetPassName = "";
		this->TargetPass = null;
	}

	GStep::GStep(const std::string& renderQueuePassName)
	{
		this->InitializeStep(renderQueuePassName);
	}

	GStep::GStep(const GStep& other)
	{
		this->ApplicableList = other.ApplicableList;

		this->TargetPassName = other.TargetPassName;
		this->TargetPass = other.TargetPass;
	}

	GStep::~GStep()
	{
		for (auto& applicale : this->ApplicableList)
		{
			applicale.reset();
			applicale = null;
		}
		this->ApplicableList.clear();

		this->TargetPassName = "";
		this->TargetPass = null;
	}

	void GStep::InitializeStep(const std::string& renderQueuePassName)
	{
		this->TargetPassName = renderQueuePassName;
	}

	void GStep::AddApplicable(std::shared_ptr<GApplicable> applicable)
	{
		this->ApplicableList.push_back(applicable);
	}

	void GStep::SetParent(const GRenderable& renderable)
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->SetParent(renderable);
		}
	}

	void GStep::Apply()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}
	}

	void GStep::Submit(std::shared_ptr<GRenderable> renderable)
	{
		this->TargetPass->Accept(GTask::CreateNewTask(std::make_shared<GStep>(*this), renderable));
	}

	void GStep::Link(std::string renderGraphName)
	{
		auto& RenderGraph = GRenderer::GetRenderGraph(renderGraphName);

		if (auto pass = dynamic_cast<GRenderQueuePass*>(RenderGraph->GetPass(this->TargetPassName).get()))
		{
			this->TargetPass.reset(pass);
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The pass named '{}' is NOT a render queue pass!", this->TargetPassName));
		}
	}
}

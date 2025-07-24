#include "GApplyPass.h"

namespace GE
{
	GApplyPass::GApplyPass()
	{
		this->ApplicableList.clear();
	}

	GApplyPass::GApplyPass(const std::string& name, std::vector<std::shared_ptr<GApplicable>> applicables)
	{
		this->InitializeApplyPass(name, applicables);
	}

	GApplyPass::GApplyPass(const GApplyPass& other)
	{
		this->ApplicableList = other.ApplicableList;
		this->Framebuffer = other.Framebuffer;
	}

	GApplyPass::~GApplyPass()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
			applicable = null;
		}
		this->ApplicableList.clear();
	}

	void GApplyPass::InitializeApplyPass(const std::string& name, std::vector<std::shared_ptr<GApplicable>> applicables)
	{
		this->PassName = name;

		this->ApplicableList = applicables;
	}

	void GApplyPass::AddApplicable(std::shared_ptr<GApplicable> applicable)
	{
		this->ApplicableList.push_back(applicable);
	}

	void GApplyPass::Apply()
	{
		this->Framebuffer->ApplyFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}
	}

	void GApplyPass::Finalize()
	{
		GPass::Finalize();
		if (!this->Framebuffer)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The apply pass must need a framebuffer!");
		}
	}
}

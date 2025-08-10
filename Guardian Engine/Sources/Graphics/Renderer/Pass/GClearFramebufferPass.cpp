#include "GClearFramebufferPass.h"

namespace GE
{
	GClearFramebufferPass::GClearFramebufferPass()
	{
		this->ClearingFramebuffer = null;
	}

	GClearFramebufferPass::GClearFramebufferPass(const std::string& name)
	{
		this->InitializeClearFramebufferPass(name);

		this->RegisterSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink(
			"ClearingFramebuffer", this->ClearingFramebuffer));
		this->RegisterSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource(
			"ClearingFramebuffer", this->ClearingFramebuffer));
	}

	GClearFramebufferPass::GClearFramebufferPass(const GClearFramebufferPass& other) : GPass(other)
	{
		this->ClearingFramebuffer = other.ClearingFramebuffer;
	}

	GClearFramebufferPass::~GClearFramebufferPass()
	{
		this->ClearingFramebuffer = null;
	}

	void GClearFramebufferPass::InitializeClearFramebufferPass(const std::string& name)
	{
		this->PassName = name;
	}

	void GClearFramebufferPass::Execute()
	{
		this->ClearingFramebuffer->BeginRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		this->ClearingFramebuffer->ClearFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		this->ClearingFramebuffer->EndUpRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
	}

	void GClearFramebufferPass::Finalize()
	{
		GPass::Finalize();
		if (!this->ClearingFramebuffer)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The clear framebuffer pass must need a framebuffer!");
		}
	}
}

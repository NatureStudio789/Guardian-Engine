#include "GRenderGraph.h"

namespace GE
{
	GRenderGraph::GRenderGraph()
	{
		this->RenderGraphId = GUUID();
		this->IsFinalized = false;
	}

	GRenderGraph::GRenderGraph(const std::string& name) : GRenderGraph()
	{
		this->InitializeRenderGraph(name);
	}

	GRenderGraph::GRenderGraph(const GRenderGraph& other)
	{
		this->RenderGraphFramebuffer = other.RenderGraphFramebuffer;
		this->RenderGraphCamera = other.RenderGraphCamera;

		this->RenderGraphName = other.RenderGraphName;
		this->RenderGraphId = other.RenderGraphId;

		this->IsFinalized = other.IsFinalized;
		this->GlobalSinkList = other.GlobalSinkList;
		this->GlobalSourceList = other.GlobalSourceList;

		this->PassList = other.PassList;
	}

	GRenderGraph::~GRenderGraph()
	{
		this->RenderGraphName.clear();
		this->RenderGraphId = 0;

		this->IsFinalized = false;
		for (auto& sink : this->GlobalSinkList)
		{
			sink.reset();
			sink = null;
		}
		this->GlobalSinkList.clear();
		for (auto& source : this->GlobalSourceList)
		{
			source.reset();
			source = null;
		}
		this->GlobalSourceList.clear();

		for (auto& pass : this->PassList)
		{
			pass.second.reset();
			pass.second = null;
		}
		this->PassList.clear();
	}

	void GRenderGraph::InitializeRenderGraph(const std::string& name)
	{
		this->RenderGraphName = name;
		this->RenderGraphId = GUUID();

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->RegisterGraphicsCommandList(this->RenderGraphName);

		this->RenderGraphFramebuffer = GFramebuffer::CreateNewFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext());

		this->RenderGraphCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, -15.0f), GVector3(), GPerspectiveProjection());

		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("Framebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("Framebuffer", this->RenderGraphFramebuffer));

		this->AddGlobalSource(GDirectCameraSource::CreateNewDirectCameraSource("Camera", this->RenderGraphCamera));
		this->AddGlobalSink(GDirectCameraSink::CreateNewDirectCameraSink("Camera", this->RenderGraphCamera));
	}

	void GRenderGraph::SetCamera(std::shared_ptr<GCamera> camera)
	{
		GUARDIAN_CHECK_POINTER(camera);

		this->RenderGraphCamera->Position = camera->Position;
		this->RenderGraphCamera->Rotation = camera->Rotation;
		this->RenderGraphCamera->Projection = camera->Projection;
		this->RenderGraphCamera->IsFreeLook = camera->IsFreeLook;
	}

	void GRenderGraph::Execute()
	{
		if (!this->IsFinalized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The render graph has NOT been finalized!");
		}

		this->InitializeGraphGraphics();

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->BeginRendering();

		for (auto& pass : this->PassList)
		{
			pass.second->Execute();
		}

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->EndUpRendering();
	}

	void GRenderGraph::Reset()
	{
		if (!this->IsFinalized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The render graph has NOT been finalized!");
		}

		this->InitializeGraphGraphics();

		for (auto& pass : this->PassList)
		{
			pass.second->Reset();
		}
	}

	const GUUID& GRenderGraph::GetRenderGraphId() const noexcept
	{
		return this->RenderGraphId;
	}

	const std::string& GRenderGraph::GetRenderGraphName() const noexcept
	{
		return this->RenderGraphName;
	}

	std::shared_ptr<GCamera> GRenderGraph::GetRenderGraphCamera()
	{
		return this->RenderGraphCamera;
	}

	std::shared_ptr<GPass> GRenderGraph::GetPass(const std::string& passName)
	{
		if (!this->PassList.count(passName))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No pass named '{}' found in render graph!", passName));
		}

		return this->PassList[passName];
	}

	void GRenderGraph::InitializeGraphGraphics()
	{
		GGraphicsContextRegistry::GetCurrentGraphicsContext()->SetCurrentCommandList(this->RenderGraphName);
	}

	void GRenderGraph::SetSinkTarget(const std::string& sinkName, const std::string& target)
	{
		const auto finder = [&sinkName](const std::shared_ptr<GSink> sink)
		{
			return sink->GetSinkName() == sinkName;
		};
		const auto i = std::find_if(this->GlobalSinkList.begin(), this->GlobalSinkList.end(), finder);
		if (i == this->GlobalSinkList.end())
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No global sink named '{}' found", sinkName));
		}

		auto TargetSplit = GUtil::SplitString(target, ".");
		if (TargetSplit.size() != (size_t)2)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Sink target format is invalid");
		}
		(*i)->SetTarget(TargetSplit[0], TargetSplit[1]);
	}

	void GRenderGraph::AddGlobalSource(std::shared_ptr<GSource> source)
	{
		this->GlobalSourceList.push_back(source);
	}

	void GRenderGraph::AddGlobalSink(std::shared_ptr<GSink> sink)
	{
		this->GlobalSinkList.push_back(sink);
	}

	void GRenderGraph::Finalize()
	{
		if (this->IsFinalized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The render graph has been finalized!");
		}

		for (auto& pass : this->PassList)
		{
			pass.second->Finalize();
		}
		this->LinkGlobalSinks();
		this->IsFinalized = true;
	}

	void GRenderGraph::AppendPass(std::shared_ptr<GPass> pass)
	{
		if (this->IsFinalized)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The render graph has been finalized!");
		}

		if (this->PassList.count(pass->GetPassName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The pass named '{}' already exists in render graph!", pass->GetPassName()));
		}

		this->LinkSinks(pass);

		this->PassList[pass->GetPassName()] = pass;
	}

	void GRenderGraph::LinkSinks(std::shared_ptr<GPass> pass)
	{
		for (auto& sink : pass->GetSinkList())
		{
			const auto& sinkInputPassName = sink->GetTargetPassName();

			GUARDIAN_CHECK_VALUE(!sinkInputPassName.empty());

			if (sinkInputPassName == "$")
			{
				bool applied = false;
				for (auto& source : this->GlobalSourceList)
				{
					if (source->GetSourceName() == sink->GetOutputName())
					{
						sink->Apply(source);
						applied = true;
						break;
					}
				}

				if (!applied)
				{
					throw GUARDIAN_ERROR_EXCEPTION(std::format("No output named '{}' found in global sources!", sink->GetOutputName()));
				}
			}
			else
			{
				bool applied = false;
				for (auto& epass : this->PassList)
				{
					if (epass.first == sinkInputPassName)
					{
						auto& source = epass.second->GetPassSource(sink->GetOutputName());
						sink->Apply(source);
						applied = true;
						break;
					}
				}

				if (!applied)
				{
					throw GUARDIAN_ERROR_EXCEPTION(std::format("No output named '{}' found!", sink->GetOutputName()));
				}
			}
		}
	}

	void GRenderGraph::LinkGlobalSinks()
	{
		for (auto& sink : this->GlobalSinkList)
		{
			const auto& sinkInputPassName = sink->GetTargetPassName();
			for (auto& pass : this->PassList)
			{
				if (pass.second->GetPassName() == sinkInputPassName)
				{
					auto& source = pass.second->GetPassSource(sink->GetOutputName());
					sink->Apply(source);
					break;
				}
			}
		}
	}
}

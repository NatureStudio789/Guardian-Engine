#include "GSceneRenderGraph.h"
#include "../../../Gameplay/Scene/GSceneRegistry.h"

namespace GE
{
	GSceneRenderGraph::GSceneRenderGraph(const std::string& name)
	{
		this->RenderGraphName = name;
		this->RenderGraphId = GUUID();

		GGraphicsContextRegistry::GetCurrentGraphicsContext()->RegisterGraphicsCommandList(this->RenderGraphName);

		this->RenderGraphFramebuffer = GFramebuffer::CreateNewFramebuffer(
			GGraphicsContextRegistry::GetCurrentGraphicsContext(), true);
		this->RuntimeFramebuffer = GFramebuffer::CreateNewFramebuffer(
			GGraphicsContextRegistry::GetCurrentGraphicsContext(), true);

		this->RenderGraphCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, -15.0f), GVector3(), GPerspectiveProjection());
		this->RuntimeCamera = std::make_shared<GCamera>(GVector3(0.0f, 0.0f, -15.0f), GVector3(), GPerspectiveProjection());

		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("ClearingEditFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("ClearingEditFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("EditFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("EditFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("WireframeFramebuffer", this->RenderGraphFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("WireframeFramebuffer", this->RenderGraphFramebuffer));

		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("ClearingRuntimeFramebuffer", this->RuntimeFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("ClearingRuntimeFramebuffer", this->RuntimeFramebuffer));
		this->AddGlobalSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("RuntimeFramebuffer", this->RuntimeFramebuffer));
		this->AddGlobalSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("RuntimeFramebuffer", this->RuntimeFramebuffer));

		this->AddGlobalSource(GDirectCameraSource::CreateNewDirectCameraSource("EditCamera", this->RenderGraphCamera));
		this->AddGlobalSink(GDirectCameraSink::CreateNewDirectCameraSink("EditCamera", this->RenderGraphCamera));
		this->AddGlobalSource(GDirectCameraSource::CreateNewDirectCameraSource("WireframeCamera", this->RenderGraphCamera));
		this->AddGlobalSink(GDirectCameraSink::CreateNewDirectCameraSink("WireframeCamera", this->RenderGraphCamera));
		this->AddGlobalSource(GDirectCameraSource::CreateNewDirectCameraSource("RuntimeCamera", this->RuntimeCamera));
		this->AddGlobalSink(GDirectCameraSink::CreateNewDirectCameraSink("RuntimeCamera", this->RuntimeCamera));

		{
			auto ClearEditFramebufferPass = std::make_shared<GClearFramebufferPass>("Clear Edit");
			ClearEditFramebufferPass->SetSinkLinkage("ClearingFramebuffer", "$.ClearingEditFramebuffer");

			this->AppendPass(ClearEditFramebufferPass);
		}

		{
			auto EditPass = std::make_shared<GLightingPass>("Edit");
			EditPass->SetSinkLinkage("Framebuffer", "$.EditFramebuffer");
			EditPass->SetSinkLinkage("Camera", "$.EditCamera");

			this->AppendPass(EditPass);
		}

		{
			auto WireframePass = std::make_shared<GWireframePass>("Wireframe");
			WireframePass->SetSinkLinkage("Framebuffer", "$.WireframeFramebuffer");
			WireframePass->SetSinkLinkage("Camera", "$.WireframeCamera");

			this->AppendPass(WireframePass);
		}

		{
			auto ClearRuntimeFramebufferPass = std::make_shared<GClearFramebufferPass>("Clear Runtime");
			ClearRuntimeFramebufferPass->SetSinkLinkage("ClearingFramebuffer", "$.ClearingRuntimeFramebuffer");

			this->AppendPass(ClearRuntimeFramebufferPass);
		}

		{
			auto RuntimePass = std::make_shared<GLightingPass>("Runtime");
			RuntimePass->SetSinkLinkage("Framebuffer", "$.RuntimeFramebuffer");
			RuntimePass->SetSinkLinkage("Camera", "$.RuntimeCamera");

			this->AppendPass(RuntimePass);
		}

		this->Finalize();
	}

	void GSceneRenderGraph::Execute()
	{
		this->SetCamera(GSceneRegistry::GetActiveScene()->GetEditCamera());
		this->SetRuntimeCamera(GSceneRegistry::GetActiveScene()->GetRuntimeCamera());

		GRenderGraph::Execute();
	}

	std::shared_ptr<GFramebuffer> GSceneRenderGraph::GetEditFramebuffer()
	{
		return this->RenderGraphFramebuffer;
	}

	std::shared_ptr<GFramebuffer> GSceneRenderGraph::GetRuntimeFramebuffer()
	{
		return this->RuntimeFramebuffer;
	}

	void GSceneRenderGraph::SetRuntimeCamera(std::shared_ptr<GCamera> runtimeCamera)
	{
		GUARDIAN_CHECK_POINTER(runtimeCamera);

		this->RuntimeCamera->Position = runtimeCamera->Position;
		this->RuntimeCamera->Rotation = runtimeCamera->Rotation;
		this->RuntimeCamera->Projection = runtimeCamera->Projection;
		this->RuntimeCamera->IsFreeLook = runtimeCamera->IsFreeLook;
	}
}

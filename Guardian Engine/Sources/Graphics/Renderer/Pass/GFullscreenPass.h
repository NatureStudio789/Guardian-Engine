#ifndef _GE_GFULLSCREENPASS_H_
#define _GE_GFULLSCREENPASS_H_
#include "GWireframePass.h"

namespace GE
{
	class GUARDIAN_API GFullscreenPass : public GApplyPass
	{
	public:
		GFullscreenPass(const std::string& name) : GApplyPass(name)
		{
			this->RegisterSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("SourceFramebuffer", this->SourceFramebuffer));

			this->RegisterSink(GDirectFramebufferSink::CreateNewDirectFramebufferSink("TargetFramebuffer", this->Framebuffer));
			this->RegisterSource(GDirectFramebufferSource::CreateNewDirectFramebufferSource("OutputFramebuffer", this->Framebuffer));

			this->AddApplicable(GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::FULLSCREEN_PSO));

			std::vector<GVector2> Vertices =
			{
				{ 1.0f,  1.0f},
				{ 1.0f, -1.0f},
				{-1.0f, -1.0f},
				{-1.0f,  1.0f},
			};
			std::vector<UINT> Indices =
			{
				0, 1, 2, 
				2, 3, 0
			};
			this->FrameRectangle = std::make_shared<GRenderable>("FrameRectangle",
				GVertexBuffer::CreateNewVertexBuffer((void*)Vertices.data(), (UINT)Vertices.size(), sizeof(GVector2)),
				GIndexBuffer::CreateNewIndexBuffer(Indices.data(), (UINT)Indices.size()),
				GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));
		}

		void Apply() override
		{
			GApplyPass::Apply();

			UINT SourceFramebufferParameterIndex = GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::FULLSCREEN_PSO)->
				GetPipelineRootSignature()->GetRootParameterIndex(GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, 0));
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
				SetGraphicsRootDescriptorTable(SourceFramebufferParameterIndex,
					this->SourceFramebuffer->GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle);
		}

		void Execute() override
		{
			this->Framebuffer->BeginRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->ApplyDescriptorHeaps();

			this->Apply();

			this->FrameRectangle->Apply();
			if (this->FrameRectangle->HasIndexBuffer())
			{
				GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
					DrawIndexedInstanced(this->FrameRectangle->GetIndicesCount(), 1, 0, 0, 0);
			}
			else
			{
				GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
					DrawInstanced(this->FrameRectangle->GetVerticesCount(), 1, 0, 0);
			}

			this->Framebuffer->EndUpRendering(GGraphicsContextRegistry::GetCurrentGraphicsContext());
		}

	private:
		std::shared_ptr<GFramebuffer> SourceFramebuffer;
		std::shared_ptr<GRenderable> FrameRectangle;
	};
}

#endif
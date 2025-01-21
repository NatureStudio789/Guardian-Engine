#include "GuardianRenderer.h"

namespace guardian
{
	std::map<GuardianSubmitPassLevel, std::vector<std::shared_ptr<GuardianRenderable>>> GuardianRenderer::RenderableList;
	std::shared_ptr<GuardianCamera> GuardianRenderer::RendererCamera = std::make_shared<GuardianCamera>();


	void GuardianRenderer::SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable)
	{
		RenderableList[level].push_back(renderable);
	}

	void GuardianRenderer::SetRenderingCamera(std::shared_ptr<GuardianCamera> camera)
	{
		RendererCamera = camera;
	}

	void GuardianRenderer::RenderScene(std::shared_ptr<GuardianGraphics> graphics)
	{
		for (UINT i = 0; i < (UINT)GE_SUBMIT_GUI2D; i++)
		{
			for (UINT j = 0; j < (UINT)RenderableList[(GuardianSubmitPassLevel)i].size(); j++)
			{
				RenderableList[(GuardianSubmitPassLevel)i][j]->Update();
				if (RenderableList[(GuardianSubmitPassLevel)i][j]->RenderingTransformConstantBuffer->GetConstantBufferObject().Get())
				{
					RenderableList[(GuardianSubmitPassLevel)i][j]->RenderingTransformConstantBuffer->UpdateData(
						RenderableList[(GuardianSubmitPassLevel)i][j]->RenderingTransformConstantBuffer->GetConstantBufferData() *
						RendererCamera->GetViewMatrix() * RendererCamera->GetProjectionMatrix());
				}

				RenderableList[(GuardianSubmitPassLevel)i][j]->Render(graphics);
			}
		}
	}

	void GuardianRenderer::UpdateProjectionAspect(float newWidth, float newHeight)
	{
		RendererCamera->Projection.Aspect = newWidth / newHeight;
	}
}

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
		BeginSceneRendering();

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

		EndUpSceneRendering();
	}

	void GuardianRenderer::UpdateRenderer()
	{
		if (GuardianKeyboard::IsKeyPress('W'))
		{
			RendererCamera->Translate(RendererCamera->GetForwardVector() * 1.0f);
		}
		if (GuardianKeyboard::IsKeyPress('S'))
		{
			RendererCamera->Translate(RendererCamera->GetBackwardVector() * 1.0f);
		}
		if (GuardianKeyboard::IsKeyPress('A'))
		{
			RendererCamera->Translate(RendererCamera->GetLeftVector() * 1.0f);
		}
		if (GuardianKeyboard::IsKeyPress('D'))
		{
			RendererCamera->Translate(RendererCamera->GetRightVector() * 1.0f);
		}
		if (GuardianKeyboard::IsKeyPress(VK_SPACE))
		{
			RendererCamera->Translate(GVector3(0.0f, 1.0f, 0.0f));
		}
		if (GuardianKeyboard::IsKeyPress(VK_SHIFT))
		{
			RendererCamera->Translate(GVector3(0.0f, -1.0f, 0.0f));
		}

		while (!GuardianInput::IsMouseRawMoveEventListEmpty())
		{
			GuardianMouseRawMoveEvent event = GuardianInput::ReadRawMoveEvent();
			RendererCamera->Rotate(GVector3(event.MouseMovementY * 0.01f, event.MouseMovementX * 0.01f, 0.0f));
		}
	}

	void GuardianRenderer::BeginSceneRendering()
	{

	}

	void GuardianRenderer::EndUpSceneRendering()
	{

	}

	void GuardianRenderer::UpdateProjectionAspect(float newWidth, float newHeight)
	{
		RendererCamera->Projection.Aspect = newWidth / newHeight;
	}
}

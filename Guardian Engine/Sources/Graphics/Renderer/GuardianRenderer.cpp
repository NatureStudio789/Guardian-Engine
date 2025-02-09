#include "GuardianRenderer.h"
#include "../../Application/GuardianApplication.h"

namespace guardian
{
	std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>> GuardianRenderer::RenderableList;


	void GuardianRenderer::SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable)
	{
		RenderableList[level].push(renderable);
	}

	void GuardianRenderer::RenderScene()
	{
		BeginSceneRendering();

		for (UINT i = 0; i <= (UINT)GE_SUBMIT_SPECIALLY; i++)
		{
			while (!RenderableList[(GuardianSubmitPassLevel)i].empty())
			{
				auto& renderable = RenderableList[(GuardianSubmitPassLevel)i].front();

				renderable->Update();

				renderable->Render(GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext());

				RenderableList[(GuardianSubmitPassLevel)i].pop();
			}
		}

		EndUpSceneRendering();
	}

	void GuardianRenderer::BeginSceneRendering()
	{

	}

	void GuardianRenderer::EndUpSceneRendering()
	{

	}
}

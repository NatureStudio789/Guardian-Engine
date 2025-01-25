#ifndef _GE_GUARDIANRENDERER_H_
#define _GE_GUARDIANRENDERER_H_
#include "../Renderable/Model/GuardianModel.h"

namespace guardian
{
	class GUARDIAN_API GuardianRenderer
	{
	public:
		static void SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable);
		static void SetRenderingCamera(std::shared_ptr<GuardianCamera> camera);

		static void RenderScene(std::shared_ptr<GuardianGraphics> graphics);
		static void UpdateRenderer();

	private:
		static void BeginSceneRendering();
		static void EndUpSceneRendering();

		static void UpdateProjectionAspect(float newWidth, float newHeight);

		static std::map<GuardianSubmitPassLevel, std::vector<std::shared_ptr<GuardianRenderable>>> RenderableList;
		static std::shared_ptr<GuardianCamera> RendererCamera;

		friend class GuardianApplication;
	};
}

#endif
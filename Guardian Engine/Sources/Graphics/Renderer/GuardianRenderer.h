#ifndef _GE_GUARDIANRENDERER_H_
#define _GE_GUARDIANRENDERER_H_
#include "../Renderable/Model/GuardianModel.h"

namespace guardian
{
	class GUARDIAN_API GuardianRenderer
	{
	public:
		static void SubmitRenderable(GuardianSubmitPassLevel level, std::shared_ptr<GuardianRenderable> renderable);

		static void RenderScene();

	private:
		static void BeginSceneRendering();
		static void EndUpSceneRendering();

		static std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>> RenderableList;

		friend class GuardianApplication;
	};
}

#endif
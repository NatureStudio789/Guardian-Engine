#ifndef _GE_GUARDIANRENDERER_H_
#define _GE_GUARDIANRENDERER_H_
#include "../Renderable/Geometry/GuardianGeometry.h"

namespace GE
{
	class GUARDIAN_API GuardianRenderer
	{
	public:
		static void CreateRenderingFramebuffer(const GString& framebufferName, 
			const GuardianCamera& camera, int width, int height);
		static void ResizeRenderingFramebuffer(const GString& framebufferName, int width, int height);
		static void SetFramebufferCamera(const GString& framebufferName, const GuardianCamera& camera);
		static std::shared_ptr<GuardianFramebuffer> GetRenderingFramebuffer(const GString& name);

		static void SubmitRenderable(GuardianSubmitPassLevel level, 
			const GString& submitFramebuffer, std::shared_ptr<GuardianRenderable> renderable);

		static void SetClearColor(const GVector3& color);

		static void Render();

	private:
		static std::map<GString, std::map<GuardianSubmitPassLevel, std::queue<std::shared_ptr<GuardianRenderable>>>> RenderableList;
		static std::map<GString, std::shared_ptr<GuardianFramebuffer>> RenderingFramebufferList;
		static std::map<GString, GuardianCamera> RenderingCameraList;

		static GVector3 RenderingClearColor;

		friend class GuardianApplication;
	};
}

#endif
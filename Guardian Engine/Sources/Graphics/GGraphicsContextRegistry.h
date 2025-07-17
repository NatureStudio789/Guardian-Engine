#ifndef _GE_GGRAPHICSCONTEXTREGISTRY_H_
#define _GE_GGRAPHICSCONTEXTREGISTRY_H_
#include "Framebuffer/GFramebufferRegistry.h"

namespace GE
{
	class GUARDIAN_API GGraphicsContextRegistry
	{
	public:
		static void RegisterGraphicsContext(
			std::string name, std::shared_ptr<GGraphicsContext> context);

		static void SetCurrentGraphicsContext(const std::string& name);
		static void SetCurrentGraphicsContext(const GUUID& id);

		static std::shared_ptr<GGraphicsContext> GetCurrentGraphicsContext();

		static std::shared_ptr<GGraphicsContext> GetGraphicsContext(const std::string& name);
		static std::shared_ptr<GGraphicsContext> GetGraphicsContext(const GUUID& id);

	private:
		static std::string CurrentGraphicsContext;
		static std::map<std::string, std::shared_ptr<GGraphicsContext>> GraphicsContextList;
	};
}

#endif
#ifndef _GE_FRAMEBUFFERREGISTRY_H_
#define _GE_FRAMEBUFFERREGISTRY_H_
#include "GFramebuffer.h"

namespace GE
{
	class GUARDIAN_API GFramebufferRegistry
	{
	public:
		static void InitializeFramebufferRegistry();

		static void RegisterFramebuffer(const std::string& name);

		static std::shared_ptr<GFramebuffer> GetFramebuffer(const std::string& name);
		static std::shared_ptr<GFramebuffer> GetFramebuffer(const GUUID& id);

	private:
		static std::shared_ptr<GEventProcesser> RegistryEventProcesser;
		static std::map<std::string, std::shared_ptr<GFramebuffer>> FramebufferList;
	};
}

#endif
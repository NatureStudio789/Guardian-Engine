#ifndef _GE_GUARDIANGRAPHICSREGISTRY_H_
#define _GE_GUARDIANGRAPHICSREGISTRY_H_
#include "GuardianGraphics.h"

namespace GE
{
	class GUARDIAN_API GuardianGraphicsRegistry
	{
	public:
		static void RegisterGraphics(const GString& name, std::shared_ptr<GuardianGraphics> graphics);

		static void SetCurrentGraphics(const GString& name);
		static void SetCurrentGraphics(const GuardianUUID& id);

		static std::shared_ptr<GuardianGraphics> GetCurrentGraphics();

		static std::shared_ptr<GuardianGraphics> GetGraphics(const GString& name);
		static std::shared_ptr<GuardianGraphics> GetGraphics(const GuardianUUID& id);

	private:
		static GString CurrentGraphics;
		static std::map<GString, std::shared_ptr<GuardianGraphics>> GraphicsList;
	};
}

#endif
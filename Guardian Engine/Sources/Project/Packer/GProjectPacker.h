#ifndef _GE_GPROJECTPACKER_H_
#define _GE_GPROJECTPACKER_H_
#include "GAssetPacker.h"

namespace GE
{
	class GUARDIAN_API GProject;

	class GUARDIAN_API GProjectPacker
	{
	public:
		static void PackProject(std::shared_ptr<GProject> project, const std::string& packDirectory);
	};
}

#endif
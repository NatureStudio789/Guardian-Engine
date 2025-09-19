#ifndef _GE_GRUNTIME_H_
#define _GE_GRUNTIME_H_
#include <Profile/EntryPoint/GEntryPoint.h>
#include "RenderGraph/GRuntimeRenderGraph.h"

namespace GE
{
	class GRuntime : public GProgram
	{
	public:
		GRuntime();
		~GRuntime() override;

		void PreInitializeProgram() override;
		void InitializeProgram() override;
		void UpdateProgram() override;
		void ReleaseProgram() override;

	private:
		std::string RuntimeName;
		GUUID StartupSceneId;
		std::string AssetPackage;

		std::shared_ptr<GEventProcesser> RuntimeEventProcesser;

		std::shared_ptr<GAssetLoader> RuntimeAssetLoader;
		std::shared_ptr<GScene> RuntimeScene;
	};
}

GUARDIAN_SUBMIT_PROGRAM(GE::GRuntime)

#endif
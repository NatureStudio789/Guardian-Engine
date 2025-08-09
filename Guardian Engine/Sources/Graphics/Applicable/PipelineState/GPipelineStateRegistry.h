#ifndef _GE_GPIPELINESTATEREGISTRY_H_
#define _GE_GPIPELINESTATEREGISTRY_H_
#include "GPipelineState.h"

namespace GE
{
	class GUARDIAN_API GPipelineStateRegistry
	{
	public:
		static void InitializePipelineStateRegistry();
		static void RegisterPipelineState(std::shared_ptr<GPipelineState> pipelineState);

		static std::shared_ptr<GPipelineState> GetPipelineState(const std::string& name);

		static const std::string LIGHTING_PSO;
		static const std::string WIREFRAME_PSO;

	private:
		static std::map<std::string, std::shared_ptr<GPipelineState>> PipelineStateList;
	};
}

#endif
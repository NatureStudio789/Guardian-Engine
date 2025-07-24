#ifndef _GE_GSTEP_H_
#define _GE_GSTEP_H_
#include "../../Applicable/PipelineState/GPipelineStateRegistry.h"

namespace GE
{
	class GUARDIAN_API GRenderable;
	class GUARDIAN_API GRenderQueuePass;

	class GUARDIAN_API GStep
	{
	public:
		GStep();
		GStep(const std::string& renderQueuePassName);
		GStep(const GStep& other);
		~GStep();

		void InitializeStep(const std::string& renderQueuePassName);
		void AddApplicable(std::shared_ptr<GApplicable> applicable);

		void Apply();
		void Submit(std::shared_ptr<GRenderable> renderable);

		void Link(std::string renderGraphName);

		static std::shared_ptr<GStep> CreateNewStep(const std::string& renderQueuePassName)
		{
			return std::make_shared<GStep>(renderQueuePassName);
		}

	private:
		std::vector<std::shared_ptr<GApplicable>> ApplicableList;

		std::string TargetPassName;
		std::shared_ptr<GRenderQueuePass> TargetPass;
	};
}

#endif
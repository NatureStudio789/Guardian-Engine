#ifndef _GE_GRENDERQUEUEPASS_H_
#define _GE_GRENDERQUEUEPASS_H_
#include "GApplyPass.h"

namespace GE
{
	class GUARDIAN_API GRenderQueuePass : public GApplyPass
	{
	public:
		using GApplyPass::GApplyPass;

		void Accept(std::shared_ptr<GTask> task);

		void Execute() override;
		void Reset() override;

	private:
		std::vector<std::shared_ptr<GTask>> TaskList;
	};
}

#endif
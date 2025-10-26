#ifndef _GE_GRENDERQUEUEPASS_H_
#define _GE_GRENDERQUEUEPASS_H_
#include "GApplyPass.h"

namespace GE
{
	class GUARDIAN_API GRenderQueuePass : public GApplyPass
	{
	public:
		using GApplyPass::GApplyPass;

		virtual void Accept(std::shared_ptr<GTask> task);

		virtual void Execute() override;
		virtual void Reset() override;

	protected:
		std::vector<std::shared_ptr<GTask>> TaskList;

		friend class GBillboardPass;
	};
}

#endif
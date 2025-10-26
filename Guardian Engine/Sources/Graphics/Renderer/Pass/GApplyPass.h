#ifndef _GE_GAPPLYPASS_H_
#define _GE_GAPPLYPASS_H_
#include "GClearFramebufferPass.h"

namespace GE
{
	class GUARDIAN_API GApplyPass : public GPass
	{
	public:
		GApplyPass();
		GApplyPass(const std::string& name, 
			std::vector<std::shared_ptr<GApplicable>> applicables = {});
		GApplyPass(const GApplyPass& other);
		~GApplyPass();

		void InitializeApplyPass(const std::string& name, 
			std::vector<std::shared_ptr<GApplicable>> applicables = {});
		void AddApplicable(std::shared_ptr<GApplicable> applicable);

		virtual void Apply();
		virtual void Finalize() override;

	protected:
		std::shared_ptr<GFramebuffer> Framebuffer;

		std::vector<std::shared_ptr<GApplicable>> ApplicableList;
	};
}

#endif
#ifndef _GE_GCLEARFRAMEBUFFERPASS_H_
#define _GE_GCLEARFRAMEBUFFERPASS_H_
#include "GPass.h"

namespace GE
{
	class GUARDIAN_API GClearFramebufferPass : public GPass
	{
	public:
		GClearFramebufferPass();
		GClearFramebufferPass(const std::string& name);
		GClearFramebufferPass(const GClearFramebufferPass& other);
		~GClearFramebufferPass();

		void InitializeClearFramebufferPass(const std::string& name);

		void Execute() override;
		void Finalize() override;

		static std::shared_ptr<GClearFramebufferPass> CreateNewClearFramebufferPass(const std::string& name)
		{
			return std::make_shared<GClearFramebufferPass>(name);
		}

	private:
		std::shared_ptr<GFramebuffer> ClearingFramebuffer;
	};
}

#endif
#ifndef _GE_GSHADERVIEWAPPLICATOR_H_
#define _GE_GSHADERVIEWAPPLICATOR_H_
#include "../Sampler/GSampler.h"

namespace GE
{
	class GUARDIAN_API GShaderViewApplicator : public GApplicable
	{
	public:
		GShaderViewApplicator();
		GShaderViewApplicator(const std::string& shaderViewName);
		GShaderViewApplicator(const GUUID& shaderViewId);
		GShaderViewApplicator(const GShaderViewApplicator& other);
		~GShaderViewApplicator() override;

		void SetApplyingIdentification(const std::string& name);
		void SetApplyingIdentification(const GUUID& id);

		void Apply() override;

		const std::string& GetApplyingName() const;
		const GUUID& GetApplyingId() const;

		static std::shared_ptr<GShaderViewApplicator> CreateNewShaderViewApplicator(const std::string& shaderViewName)
		{
			return std::make_shared<GShaderViewApplicator>(shaderViewName);
		}

		static std::shared_ptr<GShaderViewApplicator> CreateNewShaderViewApplicator(const GUUID& shaderViewId)
		{
			return std::make_shared<GShaderViewApplicator>(shaderViewId);
		}

	private:
		std::variant<std::string, GUUID> ApplyingIdentification;
	};
}

#endif
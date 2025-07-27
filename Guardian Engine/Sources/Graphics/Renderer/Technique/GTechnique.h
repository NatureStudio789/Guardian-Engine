#ifndef _GE_GTECHNIQUE_H_
#define _GE_GTECHNIQUE_H_
#include "../Step/GStep.h"

namespace GE
{
	class GUARDIAN_API GRenderable;

	class GUARDIAN_API GTechnique
	{
	public:
		GTechnique();
		GTechnique(const std::string& name, 
			const std::string& channel, bool isActive = true);
		GTechnique(const GTechnique& other);
		~GTechnique();

		void InitializeTechnique(const std::string& name,
			const std::string& channel, bool isActive = true);
		void AddStep(std::shared_ptr<GStep> step);

		void SetParent(const GRenderable& renderable);

		void SetActiveState(bool active);

		void Submit(std::shared_ptr<GRenderable> renderable, std::string channel);
		void Link(std::string renderGraphName);

		const bool& GetActive() const noexcept;

		static std::shared_ptr<GTechnique> CreateNewTechnique(const std::string& name,
			const std::string& channel, bool isActive = true)
		{
			return std::make_shared<GTechnique>(name, channel, isActive);
		}

	private:
		std::string TechniqueName;
		std::string TechniqueChannel;
		bool IsTechniqueActive;

		std::vector<std::shared_ptr<GStep>> StepList;
	};
}

#endif
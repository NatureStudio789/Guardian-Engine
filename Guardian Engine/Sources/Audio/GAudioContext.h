#ifndef _GE_GAUDIOCONTEXT_H_
#define _GE_GAUDIOCONTEXT_H_
#include "../Aplication/Event/GMouseEvent.h"

namespace GE
{
	class GAudioContext
	{
	public:
		GAudioContext();
		GAudioContext(const std::string& name);
		GAudioContext(const GAudioContext& other);
		~GAudioContext();

		void InitializeAudioContext(const std::string& name);

		void Update();

		const GUUID& GetContextId() const noexcept;
		const std::string& GetContextName() const noexcept;
		std::shared_ptr<DirectX::AudioEngine> GetAudioEngine();

		static std::shared_ptr<GAudioContext> CreateNewContext(const std::string& name)
		{
			return std::make_shared<GAudioContext>(name);
		}

	private:
		GUUID ContextId;
		std::string ContextName;

		std::shared_ptr<DirectX::AudioEngine> AudioEngine;
	};
}

#endif
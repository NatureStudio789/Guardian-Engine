#ifndef _GE_GUARDIANSCRIPTABLEENTITY_H_
#define _GE_GUARDIANSCRIPTABLEENTITY_H_
#include "GuardianEntity.h"

namespace guardian
{
	class GuardianScriptableEntity
	{
	public:
		virtual ~GuardianScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return this->EntityObject.GetComponent<T>();
		}

		template<typename T>
		bool HasComponent()
		{
			return this->EntityObject.HasComponent<T>();
		}

	protected:
		virtual void Initialize() {}
		virtual void Update(GuardianTimestep deltaTime) {}
		virtual void Release() {}

	private:
		GuardianEntity EntityObject;

		friend class GuardianScene;
	};
}

#endif
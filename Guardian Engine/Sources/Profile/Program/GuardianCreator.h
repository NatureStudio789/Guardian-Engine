#ifndef _GE_GUARDIANCREATOR_H_
#define _GE_GUARDIANCREATOR_H_
#include "../../ECS/Entity/GuardianScriptableEntity.h"

namespace GE
{
	class GUARDIAN_API GuardianCreator
	{
	public:
		GuardianCreator()
		{

		}
		virtual ~GuardianCreator()
		{

		}

		virtual void Initialize() {}
		virtual void Update() {}
		virtual void Release() {}

	protected:

	private:
		virtual void Render() {}
		virtual LRESULT CALLBACK EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }

		friend class GuardianEngine;
		friend class GuardianApplication;
		friend class GuardianEditorCreator;
	};
}

#endif
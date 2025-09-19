#ifndef _GE_GPHYSICSERRORCALLBACK_H_
#define _GE_GPHYSICSERRORCALLBACK_H_
#include "../../Graphics/Renderer/GRenderer.h"

namespace GE
{
	class GUARDIAN_API GPhysicsErrorCallback : public PxDefaultErrorCallback
	{
	public:
		void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override;
	};
}

#endif
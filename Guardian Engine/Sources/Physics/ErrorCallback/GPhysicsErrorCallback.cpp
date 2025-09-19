#include "GPhysicsErrorCallback.h"

namespace GE
{
	void GPhysicsErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		throw GErrorException(line, file, message);
	}
}

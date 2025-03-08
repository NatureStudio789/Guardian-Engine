#ifndef _GE_GUARDIANSERIALIZABLE_H_
#define _GE_GUARDIANSERIALIZABLE_H_
#include "../UUID/GuardianUUID.h"

namespace guardian
{
	class GUARDIAN_API GuardianSerializable
	{
	public:
		GuardianSerializable() = default;
		virtual ~GuardianSerializable() = default;

		virtual const GString Serialize() = 0;
		virtual void Deserialize(const GString& filePath) = 0;
	};
}

#endif
#ifndef _GE_GUARDIANCONVERTER_H_
#define _GE_GUARDIANCONVERTER_H_
#include "../../Profile/Time/GuardianTime.h"

namespace guardian
{
	class GUARDIAN_API GuardianConverter
	{
	public:
		static const GString WideStringToString(const GWideString& wideString);
		static const GWideString StringToWideString(const GString& string);

		static const GString HRESULTToString(HRESULT hr);

		static const XMFLOAT3 QuaternionToEulerAngles(const XMFLOAT4& quaternion);
		static const XMFLOAT4 EulerAnglesToQuaternion(const XMFLOAT3& eulerAngles);
	};
}

#endif
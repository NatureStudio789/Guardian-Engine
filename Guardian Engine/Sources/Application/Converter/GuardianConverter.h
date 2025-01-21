#ifndef _GE_GUARDIANCONVERTER_H_
#define _GE_GUARDIANCONVERTER_H_
#include "../../Profile/Core/GuardianCore.h"

namespace guardian
{
	class GUARDIAN_API GuardianConverter
	{
	public:
		static const GWideString StringToWideString(const GString& string);

		static const GString HRESULTToString(HRESULT hr);
	};
}

#endif
#ifndef _GE_GCONVERTER_H_
#define _GE_GCONVERTER_H_
#include "../../Profile/UUID/GUUID.h"

namespace GE
{
	class GUARDIAN_API GConverter
	{
	public:
		static std::wstring StringToWideString(std::string text);

		static float DegreeToAngle(float degree);
	};
}

#endif
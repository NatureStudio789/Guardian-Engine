#ifndef _GE_GUUID_H_
#define _GE_GUUID_H_
#include "../Core/GCore.h"

namespace GE
{
	class GUARDIAN_API GUUID
	{
	public:
		GUUID();
		GUUID(unsigned long long uuid);
		GUUID(const GUUID& other);
		~GUUID();

		void Generate();

		operator unsigned long long() const;

	private:
		unsigned long long UUID;
	};
}

#endif
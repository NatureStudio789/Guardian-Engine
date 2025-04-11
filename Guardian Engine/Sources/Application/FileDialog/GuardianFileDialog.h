#ifndef _GE_GUARDIANDIALOG_H_
#define _GE_GUARDIANDIALOG_H_
#include "../Exception/GuardianException.h"

namespace GE
{
	class GUARDIAN_API GuardianFileDialog
	{
	public:
		static const GString OpenFile(const char* filter);

		static const GString SaveFile(const char* filter);
	};
}

#endif
#ifndef _GE_GCORE_H_
#define _GE_GCORE_H_
#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <sstream>

#ifdef GE_PLATFORM_WINDOWS
	
		#ifdef GE_BUILD_DLL
			#define GUARDIAN_API _declspec(dllexport)
		#else
			#define GUARDIAN_API _declspec(dllimport)
		#endif

	#else 
		#error Guardian Engine Only Supports Windows Platform!
#endif

#define null nullptr

namespace GE
{

}

#endif
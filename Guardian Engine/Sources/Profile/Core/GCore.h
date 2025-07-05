#ifndef _GE_GCORE_H_
#define _GE_GCORE_H_
#include <Windows.h>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <sstream>
#include <vector>
#include <format>
#include <Uxtheme.h>
#include <dxgi.h>
#include <d3d12.h>
#include <wrl/client.h>
using namespace Microsoft;

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

#define GUARDIAN_CLEAR_MEMORY(object) ZeroMemory(&object, sizeof(object))

namespace GE
{

}

#endif
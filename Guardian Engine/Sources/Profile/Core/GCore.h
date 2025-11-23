#ifndef _GE_GCORE_H_
#define _GE_GCORE_H_
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include <format>
#include <random>
#include <functional>
#include <queue>
#include <any>
#include <variant>

#include <Windows.h>
#include <Uxtheme.h>

#include <wrl/client.h>

#include <dxgi.h>
#include <d3d12.h>
#include <DirectX 12/d3dx12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <DirectXTex/DirectXTex.h>

#include <DirectXTK/Audio.h>
#include <DirectXCollision.h>

#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

#include <ImGuizmo.h>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include <entt/entt.hpp>

#include <yaml-cpp/yaml.h>
#include <inicpp/inicpp.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <PhysX/PxPhysicsAPI.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

using namespace Microsoft;
using namespace DirectX;
using namespace physx;
using namespace vehicle2;

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
#define GUARDIAN_ARRAYSIZE(array) ARRAYSIZE(array)

#define GUARDIAN_FAILED(hr) FAILED(hr)
#define GUARDIAN_SUCCEEDED(hr) SUCCEEDED(hr)

namespace GE
{

}

#endif
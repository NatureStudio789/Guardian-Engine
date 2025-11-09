#ifndef _GE_GSHADERVIEWRESOURCE_H_
#define _GE_GSHADERVIEWRESOURCE_H_
#include "GShaderViewGroup.h"

namespace GE
{
	class GUARDIAN_API GShaderViewRegistry
	{
	public:
		static void RegistryShaderView(std::shared_ptr<GShaderView> shaderView);
		static void RegistryShaderViewGroup(std::shared_ptr<GShaderViewGroup> shaderViewGroup);

		static std::shared_ptr<GShaderView> GetShaderView(const GUUID& id);
		static std::shared_ptr<GShaderView> GetShaderView(const std::string& name);

		static std::shared_ptr<GShaderViewGroup> GetShaderViewGroup(const GUUID& id);
		static std::shared_ptr<GShaderViewGroup> GetShaderViewGroup(const std::string& name);

		static bool HasShaderView(const GUUID& id);
		static bool HasShaderView(const std::string& name);

		static bool HasShaderViewGroup(const GUUID& id);
		static bool HasShaderViewGroup(const std::string& name);

	private:
		static std::map<std::string, std::shared_ptr<GShaderView>> ShaderViewList;
		static std::map<std::string, std::shared_ptr<GShaderViewGroup>> ShaderViewGroupList;
	};
}

#endif
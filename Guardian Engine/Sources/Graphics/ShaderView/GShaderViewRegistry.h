#ifndef _GE_GSHADERVIEWRESOURCE_H_
#define _GE_GSHADERVIEWRESOURCE_H_
#include "GShaderView.h"

namespace GE
{
	class GUARDIAN_API GShaderViewRegistry
	{
	public:
		static void RegistryShaderView(std::shared_ptr<GShaderView> shaderView);

		static std::shared_ptr<GShaderView> GetShaderView(const GUUID& id);
		static std::shared_ptr<GShaderView> GetShaderView(const std::string& name);

	private:
		static std::map<std::string, std::shared_ptr<GShaderView>> ShaderViewList;
	};
}

#endif
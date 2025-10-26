#include "GShaderViewRegistry.h"

namespace GE
{
	std::map<std::string, std::shared_ptr<GShaderView>> GShaderViewRegistry::ShaderViewList;


	void GShaderViewRegistry::RegistryShaderView(std::shared_ptr<GShaderView> shaderView)
	{
		if (ShaderViewList.count(shaderView->GetShaderViewName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The shader view named '{}' already exists in registry!", shaderView->GetShaderViewName()));
		}

		ShaderViewList[shaderView->GetShaderViewName()] = shaderView;
	}

	std::shared_ptr<GShaderView> GShaderViewRegistry::GetShaderView(const GUUID& id)
	{
		for (auto& [name, shaderView] : ShaderViewList)
		{
			if (shaderView->GetShaderViewId() == id)
			{
				return shaderView;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader view with id :'{}' found in registry!", (unsigned long long)id));
	}
	std::shared_ptr<GShaderView> GShaderViewRegistry::GetShaderView(const std::string& name)
	{
		if (!ShaderViewList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader view named '{}' found in registry!", name));
		}

		return ShaderViewList[name];
	}

	bool GShaderViewRegistry::HasShaderView(const GUUID& id)
	{
		for (auto& [name, shaderView] : ShaderViewList)
		{
			if (shaderView->GetShaderViewId() == id)
			{
				return true;
			}
		}

		return false;
	}

	bool GShaderViewRegistry::HasShaderView(const std::string& name)
	{
		return ShaderViewList.count(name);
	}
}

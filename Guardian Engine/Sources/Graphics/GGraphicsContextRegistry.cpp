#include "GGraphicsContextRegistry.h"

namespace GE
{
	std::string GGraphicsContextRegistry::CurrentGraphicsContext;
	std::map<std::string, std::shared_ptr<GGraphicsContext>> GGraphicsContextRegistry::GraphicsContextList;


	void GGraphicsContextRegistry::RegisterGraphicsContext(
		std::string name, std::shared_ptr<GGraphicsContext> context)
	{
		if (GraphicsContextList.count(name) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics context named '{}' has been in registry.", name));
		}

		GraphicsContextList[name] = context;
	}

	void GGraphicsContextRegistry::SetCurrentGraphicsContext(const std::string& name)
	{
		if (GraphicsContextList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No graphics context named '{}' found.", name));
		}

		CurrentGraphicsContext = name;
	}

	void GGraphicsContextRegistry::SetCurrentGraphicsContext(const GUUID& id)
	{
		for (auto& context : GraphicsContextList)
		{
			if (context.second->GetContextId() == id)
			{
				CurrentGraphicsContext = context.first;
				return;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No graphics context with id '{}' found.", (unsigned long long)id));
	}

	std::shared_ptr<GGraphicsContext> GGraphicsContextRegistry::GetCurrentGraphicsContext()
	{
		if (GraphicsContextList.count(CurrentGraphicsContext) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("Current graphics context named '{}' is invalid.", CurrentGraphicsContext));
		}

		return GraphicsContextList[CurrentGraphicsContext];
	}

	std::shared_ptr<GGraphicsContext> GGraphicsContextRegistry::GetGraphicsContext(const std::string& name)
	{
		if (GraphicsContextList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No graphics context named '{}' found.", name));
		}

		return GraphicsContextList[name];
	}

	std::shared_ptr<GGraphicsContext> GGraphicsContextRegistry::GetGraphicsContext(const GUUID& id)
	{
		for (auto& context : GraphicsContextList)
		{
			if (context.second->GetContextId() == id)
			{
				return context.second;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No graphics context with id '{}' found.", (unsigned long long)id));
	}
}

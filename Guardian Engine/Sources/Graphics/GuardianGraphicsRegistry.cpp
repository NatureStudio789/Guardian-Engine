#include "GuardianGraphicsRegistry.h"

namespace GE
{
	GString GuardianGraphicsRegistry::CurrentGraphics;
	std::map<GString, std::shared_ptr<GuardianGraphics>> GuardianGraphicsRegistry::GraphicsList;


	void GuardianGraphicsRegistry::RegisterGraphics(const GString& name, std::shared_ptr<GuardianGraphics> graphics)
	{
		if (GraphicsList.count(name) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics named : '{}' has been registered!", name));
		}

		GraphicsList[name] = graphics;
	}

	void GuardianGraphicsRegistry::SetCurrentGraphics(const GString& name)
	{
		if (GraphicsList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics named : '{}' hasn't been registered in registry!", name));
		}

		CurrentGraphics = name;
	}

	void GuardianGraphicsRegistry::SetCurrentGraphics(const GuardianUUID& id)
	{
		for (auto& graphics : GraphicsList)
		{
			if (graphics.second->GetGraphicsId() == id)
			{
				CurrentGraphics = graphics.first;
				return;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics with id : '{}' hasn't been registered in registry!", (unsigned long long)id));
	}

	std::shared_ptr<GuardianGraphics> GuardianGraphicsRegistry::GetCurrentGraphics()
	{
		if (GraphicsList.count(CurrentGraphics) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Current No Graphics Available!");
		}

		return GraphicsList[CurrentGraphics];
	}

	std::shared_ptr<GuardianGraphics> GuardianGraphicsRegistry::GetGraphics(const GString& name)
	{
		if (GraphicsList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics named : '{}' hasn't been registered in registry!", name));
		}

		return GraphicsList[name];
	}

	std::shared_ptr<GuardianGraphics> GuardianGraphicsRegistry::GetGraphics(const GuardianUUID& id)
	{
		for (auto& graphics : GraphicsList)
		{
			if (graphics.second->GetGraphicsId() == id)
			{
				return graphics.second;
				break;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("The graphics with id : '{}' hasn't been registered in registry!", (unsigned long long)id));
	}
}

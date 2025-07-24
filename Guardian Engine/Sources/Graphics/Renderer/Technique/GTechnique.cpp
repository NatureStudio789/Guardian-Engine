#include "GTechnique.h"

namespace GE
{
	GTechnique::GTechnique()
	{
		this->TechniqueName = "";
		this->TechniqueChannel = "";
		this->IsTechniqueActive = false;

		this->StepList.clear();
	}

	GTechnique::GTechnique(const std::string& name, const std::string& channel, bool isActive)
	{
		this->InitializeTechnique(name, channel, isActive);
	}

	GTechnique::GTechnique(const GTechnique& other)
	{
		this->TechniqueName = other.TechniqueName;
		this->TechniqueChannel = other.TechniqueChannel;
		this->IsTechniqueActive = other.IsTechniqueActive;

		this->StepList = other.StepList;
	}

	GTechnique::~GTechnique()
	{
		this->TechniqueName = "";
		this->TechniqueChannel = "";
		this->IsTechniqueActive = false;

		for (auto& step : this->StepList)
		{
			step.reset();
			step = null;
		}
		this->StepList.clear();
	}

	void GTechnique::InitializeTechnique(const std::string& name, const std::string& channel, bool isActive)
	{
		this->TechniqueName = name;
		this->TechniqueChannel = channel;
		this->IsTechniqueActive = isActive;
	}
	
	void GTechnique::Submit(std::shared_ptr<GRenderable> renderable, std::string channel)
	{
		if (this->IsTechniqueActive && channel == this->TechniqueChannel)
		{
			for (auto& step : this->StepList)
			{
				step->Submit(renderable);
			}
		}
	}

	void GTechnique::Link(std::string renderGraphName)
	{
		for (auto& step : this->StepList)
		{
			step->Link(renderGraphName);
		}
	}
}

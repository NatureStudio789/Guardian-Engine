#include "GShaderViewApplicator.h"

namespace GE
{
	GShaderViewApplicator::GShaderViewApplicator()
	{

	}

	GShaderViewApplicator::GShaderViewApplicator(const std::string& shaderViewName)
	{
		this->SetApplyingIdentification(shaderViewName);
	}

	GShaderViewApplicator::GShaderViewApplicator(const GUUID& shaderViewId)
	{
		this->SetApplyingIdentification(shaderViewId);
	}

	GShaderViewApplicator::GShaderViewApplicator(const GShaderViewApplicator& other) : GApplicable(other)
	{
		this->ApplyingIdentification = other.ApplyingIdentification;
	}

	GShaderViewApplicator::~GShaderViewApplicator()
	{

	}

	void GShaderViewApplicator::SetApplyingIdentification(const std::string& name)
	{
		this->ApplyingIdentification = name;
	}

	void GShaderViewApplicator::SetApplyingIdentification(const GUUID& id)
	{
		this->ApplyingIdentification = id;
	}

	void GShaderViewApplicator::Apply()
	{
		if (GUUID* id = std::get_if<GUUID>(&this->ApplyingIdentification))
		{
			GShaderViewRegistry::GetShaderView(*id)->ApplyShaderView();
		}
		else if (std::string* name = std::get_if<std::string>(&this->ApplyingIdentification))
		{
			GShaderViewRegistry::GetShaderView(*name)->ApplyShaderView();
		}
		else
		{
			throw GUARDIAN_ERROR_EXCEPTION("Invalid shader view identification!");
		}
	}

	const std::string& GShaderViewApplicator::GetApplyingName() const
	{
		return std::get<std::string>(this->ApplyingIdentification);
	}

	const GUUID& GShaderViewApplicator::GetApplyingId() const
	{
		return std::get<GUUID>(this->ApplyingIdentification);
	}
}

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
			if (GShaderViewRegistry::HasShaderView(*id))
			{
				GShaderViewRegistry::GetShaderView(*id)->ApplyShaderView();
			}
			else if (GShaderViewRegistry::HasShaderViewGroup(*id))
			{
				GShaderViewRegistry::GetShaderViewGroup(*id)->ApplyShaderViewGroup();
			}
			else
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader view or shader view group with id : '{}' exists!", (unsigned long long)*id));
			}
		}
		else if (std::string* name = std::get_if<std::string>(&this->ApplyingIdentification))
		{
			if (GShaderViewRegistry::HasShaderView(*name))
			{
				GShaderViewRegistry::GetShaderView(*name)->ApplyShaderView();
			}
			else if (GShaderViewRegistry::HasShaderViewGroup(*name))
			{
				GShaderViewRegistry::GetShaderViewGroup(*name)->ApplyShaderViewGroup();
			}
			else
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader view or shader view group named '{}' exists!", *name));
			}
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

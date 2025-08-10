#include "GSource.h"

namespace GE
{
	GSource::GSource()
	{
		this->SourceName = "";
	}

	GSource::GSource(const std::string& name)
	{
		this->InitializeSource(name);
	}

	GSource::GSource(const GSource& other)
	{
		this->SourceName = other.SourceName;
	}

	GSource::~GSource()
	{
		this->SourceName.clear();
	}

	void GSource::InitializeSource(const std::string& name)
	{
		this->SourceName = name;
	}

	void GSource::CheckLinkValidate() const
	{

	}

	const std::string& GSource::GetSourceName() const noexcept
	{
		return this->SourceName;
	}

	std::shared_ptr<GApplicable>& GSource::YieldApplicable()
	{
		throw GUARDIAN_ERROR_EXCEPTION("This source don't support applicable!");
	}

	std::shared_ptr<GFramebuffer>& GSource::YieldFramebuffer()
	{
		throw GUARDIAN_ERROR_EXCEPTION("This source don't support framebuffer!");
	}

	std::shared_ptr<GCamera>& GSource::YieldCamera()
	{
		throw GUARDIAN_ERROR_EXCEPTION("This source don't support camera!");
	}
}
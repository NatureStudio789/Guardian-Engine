#include "GPass.h"

namespace GE
{
	GPass::GPass()
	{
		this->PassName.clear();
	}

	GPass::GPass(const std::string& name)
	{
		this->PassName = name;
	}

	GPass::GPass(const GPass& other)
	{
		this->PassName = other.PassName;
	}

	GPass::~GPass()
	{
		this->PassName.clear();
	}

	void GPass::Execute()
	{

	}

	void GPass::Release()
	{

	}

	const std::string& GPass::GetPassName() const noexcept
	{
		return this->PassName;
	}
}

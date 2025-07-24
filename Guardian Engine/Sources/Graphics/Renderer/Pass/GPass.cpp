#include "GPass.h"

namespace GE
{
	GPass::GPass()
	{
		this->PassName.clear();
		
		this->SinkList.clear();
		this->SourceList.clear();
	}

	GPass::GPass(const std::string& name)
	{
		this->PassName = name;

		this->SinkList.clear();
		this->SourceList.clear();
	}

	GPass::GPass(const GPass& other)
	{
		this->PassName = other.PassName;

		this->SourceList = other.SourceList;
		this->SinkList = other.SinkList;
	}

	GPass::~GPass()
	{
		this->PassName.clear();

		this->SourceList.clear();
		this->SinkList.clear();
	}

	void GPass::SetSinkLinkage(const std::string& sinkName, const std::string& target)
	{
		auto sink = this->GetPassSink(sinkName);

		std::vector<std::string> TargetSplit = GUtil::SplitString(target, ".");
		if (TargetSplit.size() != (size_t)2)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The sink target format is invalid!");
		}
		
		sink->SetTarget(TargetSplit[0], TargetSplit[1]);
	}

	void GPass::Execute()
	{

	}

	void GPass::Reset()
	{

	}

	void GPass::Finalize()
	{
		for (auto& source : this->SourceList)
		{
			source->CheckLinkValidate();
		}

		for (auto& sink : this->SinkList)
		{
			sink->CheckLinkValidate();
		}
	}

	const std::string& GPass::GetPassName() const noexcept
	{
		return this->PassName;
	}

	const std::vector<std::shared_ptr<GSink>>& GPass::GetSinkList() const noexcept
	{
		return this->SinkList;
	}

	std::shared_ptr<GSource> GPass::GetPassSource(const std::string& name)
	{
		for (auto& source : this->SourceList)
		{
			if (source->GetSourceName() == name)
			{
				return source;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No source named '{}' found in pass '{}'", name, this->PassName));
	}

	std::shared_ptr<GSink> GPass::GetPassSink(const std::string& name)
	{
		for (auto& sink : this->SinkList)
		{
			if (sink->GetSinkName() == name)
			{
				return sink;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No sink named '{}' found in pass '{}'", name, this->PassName));
	}

	void GPass::RegisterSource(std::shared_ptr<GSource> source)
	{
		for (auto& s : this->SourceList)
		{
			if (s->GetSourceName() == source->GetSourceName())
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("The source named '{}' has already been in pass '{}'",
					s->GetSourceName(), this->PassName));
			}
		}

		this->SourceList.push_back(source);
	}

	void GPass::RegisterSink(std::shared_ptr<GSink> sink)
	{
		for (auto& s : this->SinkList)
		{
			if (s->GetSinkName() == sink->GetSinkName())
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("The sink named '{}' has already been in pass '{}'",
					s->GetSinkName(), this->PassName));
			}
		}

		this->SinkList.push_back(sink);
	}
}

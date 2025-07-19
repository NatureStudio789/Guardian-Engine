#include "GRenderGraph.h"

namespace GE
{
	GRenderGraph::GRenderGraph()
	{

	}

	GRenderGraph::GRenderGraph(const GRenderGraph& other)
	{
		this->PassList = other.PassList;
	}

	GRenderGraph::~GRenderGraph()
	{
		for (auto& pass : this->PassList)
		{
			pass.second.reset();
			pass.second = null;
		}
		this->PassList.clear();
	}

	void GRenderGraph::Execute()
	{
		for (auto& pass : this->PassList)
		{
			pass.second->Execute();
		}
	}

	void GRenderGraph::Release()
	{
		for (auto& pass : this->PassList)
		{
			pass.second->Release();
		}
	}

	void GRenderGraph::AppendPass(std::shared_ptr<GPass> pass)
	{
		if (this->PassList.count(pass->GetPassName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The pass named '{}' already exists in render graph!"));
		}

		this->PassList[pass->GetPassName()] = pass;
	}
}

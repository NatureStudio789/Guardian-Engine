#include "GPipelineStateRegistry.h"

namespace GE
{
	const std::string GPipelineStateRegistry::MESH_PSO = "Mesh PSO";

	std::map<std::string, std::shared_ptr<GPipelineState>> GPipelineStateRegistry::PipelineStateList;


	void GPipelineStateRegistry::InitializePipelineStateRegistry()
	{
		{
			auto RootSignature = std::make_shared<GRootSignature>();

			RootSignature;

			auto MeshPipelineState = std::make_shared<GPipelineState>(MESH_PSO);
		}
	}

	void GPipelineStateRegistry::RegisterPipelineState(std::shared_ptr<GPipelineState> pipelineState)
	{
		if (PipelineStateList.count(pipelineState->GetPipelineStateName()))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The pipeline state named '{}' already exists in registry!",
				pipelineState->GetPipelineStateName()));
		}

		PipelineStateList[pipelineState->GetPipelineStateName()] = pipelineState;
	}

	std::shared_ptr<GPipelineState> GPipelineStateRegistry::GetPipelineState(const std::string& name)
	{
		if (!PipelineStateList.count(name))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No pipeline state named '{}' found in registry!", name));
		}

		return PipelineStateList[name];
	}
}

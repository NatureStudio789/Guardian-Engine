#include "GPipelineStateRegistry.h"

namespace GE
{
	const std::string GPipelineStateRegistry::LIGHTING_PSO = "Lighting PSO";

	std::map<std::string, std::shared_ptr<GPipelineState>> GPipelineStateRegistry::PipelineStateList;


	void GPipelineStateRegistry::InitializePipelineStateRegistry()
	{
		{
			auto MeshPipelineState = std::make_shared<GPipelineState>(LIGHTING_PSO);
			MeshPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Default.gvs"));
			MeshPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Default.gps"));

			std::vector<D3D12_INPUT_ELEMENT_DESC> IED =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0, }
			};

			MeshPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			MeshPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

			GRootSignature::RootParameter Parameter;
			Parameter.Type = GRootSignature::GE_PARAMETER_CBV;
			Parameter.ShaderRegisterIndex = 0;

			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(Parameter);

			MeshPipelineState->InitializePipelineState(1, 0);
			PipelineStateList[LIGHTING_PSO] = MeshPipelineState;
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

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
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0, },
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0, },
			};

			MeshPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			MeshPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

			GRootSignature::RootParameter CBParameter;
			CBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			CBParameter.ShaderRegisterIndex = 0;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(CBParameter);
			
			GRootSignature::RootParameter TexParameter;
			TexParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			TexParameter.ShaderRegisterIndex = 0;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(TexParameter);
			
			GRootSignature::StaticSamplerDescription Sampler;
			GUARDIAN_CLEAR_MEMORY(Sampler);
			Sampler.ShaderRegister = 0;
			Sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			Sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MeshPipelineState->GetPipelineRootSignature()->AddSamplerDescription(Sampler);

			MeshPipelineState->InitializePipelineState(1, 1, 1);
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

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
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0, },
			};

			MeshPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			MeshPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));
			
			GRootSignature::RootParameter TransformCBParameter;
			TransformCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			TransformCBParameter.ShaderRegisterIndex = 0;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(TransformCBParameter);
			GRootSignature::RootParameter CameraCBParameter;
			CameraCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			CameraCBParameter.ShaderRegisterIndex = 1;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(CameraCBParameter);
			GRootSignature::RootParameter MaterialCBParameter;
			MaterialCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			MaterialCBParameter.ShaderRegisterIndex = 2;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(MaterialCBParameter);
			
			GRootSignature::RootParameter AlbedoParameter;
			AlbedoParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			AlbedoParameter.ShaderRegisterIndex = 0;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(AlbedoParameter);
			GRootSignature::RootParameter RoughnessParameter;
			RoughnessParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			RoughnessParameter.ShaderRegisterIndex = 1;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(RoughnessParameter);
			GRootSignature::RootParameter MetallicParameter;
			MetallicParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			MetallicParameter.ShaderRegisterIndex = 2;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(MetallicParameter);
			GRootSignature::RootParameter AoParameter;
			AoParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			AoParameter.ShaderRegisterIndex = 3;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(AoParameter);
			GRootSignature::RootParameter NormalParameter;
			NormalParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			NormalParameter.ShaderRegisterIndex = 4;
			MeshPipelineState->GetPipelineRootSignature()->AddRootParameter(NormalParameter);
			
			GRootSignature::StaticSamplerDescription Sampler;
			GUARDIAN_CLEAR_MEMORY(Sampler);
			Sampler.ShaderRegister = 0;
			Sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			Sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			MeshPipelineState->GetPipelineRootSignature()->AddSamplerDescription(Sampler);

			MeshPipelineState->InitializePipelineState();
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

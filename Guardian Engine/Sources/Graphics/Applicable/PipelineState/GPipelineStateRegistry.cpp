#include "GPipelineStateRegistry.h"

namespace GE
{
	const std::string GPipelineStateRegistry::LIGHTING_PSO = "Lighting PSO";
	const std::string GPipelineStateRegistry::WIREFRAME_PSO = "Wireframe PSO";
	const std::string GPipelineStateRegistry::FULLSCREEN_PSO = "Fullscreen PSO";
	const std::string GPipelineStateRegistry::DEPTH_PSO = "Depth PSO";

	std::map<std::string, std::shared_ptr<GPipelineState>> GPipelineStateRegistry::PipelineStateList;


	void GPipelineStateRegistry::InitializePipelineStateRegistry()
	{
		{
			auto LightingPipelineState = std::make_shared<GPipelineState>(LIGHTING_PSO);
			LightingPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Default.gvs"));
			LightingPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Default.gps"));

			std::vector<D3D12_INPUT_ELEMENT_DESC> IED =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{"TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			LightingPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			LightingPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));
			
			GRootSignature::RootParameter TransformCBParameter;
			TransformCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			TransformCBParameter.ShaderRegisterIndex = 0;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(TransformCBParameter);
			GRootSignature::RootParameter CameraCBParameter;
			CameraCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			CameraCBParameter.ShaderRegisterIndex = 1;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(CameraCBParameter);
			GRootSignature::RootParameter MaterialCBParameter;
			MaterialCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			MaterialCBParameter.ShaderRegisterIndex = 2;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(MaterialCBParameter);
			GRootSignature::RootParameter LightCBParameter;
			LightCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			LightCBParameter.ShaderRegisterIndex = 3;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(LightCBParameter);
			
			GRootSignature::RootParameter AlbedoParameter;
			AlbedoParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			AlbedoParameter.ShaderRegisterIndex = 0;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(AlbedoParameter);
			GRootSignature::RootParameter RoughnessParameter;
			RoughnessParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			RoughnessParameter.ShaderRegisterIndex = 1;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(RoughnessParameter);
			GRootSignature::RootParameter MetallicParameter;
			MetallicParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			MetallicParameter.ShaderRegisterIndex = 2;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(MetallicParameter);
			GRootSignature::RootParameter AoParameter;
			AoParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			AoParameter.ShaderRegisterIndex = 3;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(AoParameter);
			GRootSignature::RootParameter NormalParameter;
			NormalParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			NormalParameter.ShaderRegisterIndex = 4;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(NormalParameter);
			GRootSignature::RootParameter DepthMapParameter;
			DepthMapParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			DepthMapParameter.ShaderRegisterIndex = 5;
			LightingPipelineState->GetPipelineRootSignature()->AddRootParameter(DepthMapParameter);
			
			GRootSignature::StaticSamplerDescription Sampler;
			GUARDIAN_CLEAR_MEMORY(Sampler);
			Sampler.ShaderRegister = 0;
			Sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			Sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			LightingPipelineState->GetPipelineRootSignature()->AddSamplerDescription(Sampler);

			LightingPipelineState->InitializePipelineState();
			PipelineStateList[LIGHTING_PSO] = LightingPipelineState;
		}

		{
			auto WireframePipelineState = std::make_shared<GPipelineState>(WIREFRAME_PSO);
			WireframePipelineState->SetShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Solid.gvs"));
			WireframePipelineState->SetShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Solid.gps"));

			std::vector<D3D12_INPUT_ELEMENT_DESC> IED =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			WireframePipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			WireframePipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

			WireframePipelineState->SetRasterizerState(GRasterizerState::CreateNewRasterizerState(
				GRasterizerState::GE_FILL_WIREFRAME, GRasterizerState::GE_CULL_NONE));

			GRootSignature::RootParameter TransformCBParameter;
			TransformCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			TransformCBParameter.ShaderRegisterIndex = 0;
			WireframePipelineState->GetPipelineRootSignature()->AddRootParameter(TransformCBParameter);
			GRootSignature::RootParameter CameraCBParameter;
			CameraCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			CameraCBParameter.ShaderRegisterIndex = 1;
			WireframePipelineState->GetPipelineRootSignature()->AddRootParameter(CameraCBParameter);

			WireframePipelineState->InitializePipelineState();
			PipelineStateList[WIREFRAME_PSO] = WireframePipelineState;
		}

		{
			auto FullscreenPipelineState = std::make_shared<GPipelineState>(FULLSCREEN_PSO);
			FullscreenPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Fullscreen.gvs"));
			FullscreenPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Fullscreen.gps"));

			std::vector<D3D12_INPUT_ELEMENT_DESC> IED =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			FullscreenPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			FullscreenPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

			GRootSignature::RootParameter SourceFramebufferParameter;
			SourceFramebufferParameter.Type = GRootSignature::GE_PARAMETER_SRV;
			SourceFramebufferParameter.ShaderRegisterIndex = 0;
			FullscreenPipelineState->GetPipelineRootSignature()->AddRootParameter(SourceFramebufferParameter);

			GRootSignature::StaticSamplerDescription Sampler;
			GUARDIAN_CLEAR_MEMORY(Sampler);
			Sampler.ShaderRegister = 0;
			Sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			Sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			Sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			FullscreenPipelineState->GetPipelineRootSignature()->AddSamplerDescription(Sampler);

			FullscreenPipelineState->InitializePipelineState();
			PipelineStateList[FULLSCREEN_PSO] = FullscreenPipelineState;
		}

		{
			auto DepthPipelineState = std::make_shared<GPipelineState>(DEPTH_PSO);
			DepthPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_VERTEX_SHADER, "../Guardian Engine/Shaders/Solid.gvs"));
			DepthPipelineState->SetShader(GShader::CreateNewShader(GShader::GE_PIXEL_SHADER, "../Guardian Engine/Shaders/Solid.gps"));

			std::vector<D3D12_INPUT_ELEMENT_DESC> IED =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			DepthPipelineState->SetInputLayout(GInputLayout::CreateNewInputLayout(IED.data(), (UINT)IED.size()));

			DepthPipelineState->SetTopology(GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

			GRootSignature::RootParameter TransformCBParameter;
			TransformCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			TransformCBParameter.ShaderRegisterIndex = 0;
			DepthPipelineState->GetPipelineRootSignature()->AddRootParameter(TransformCBParameter);
			GRootSignature::RootParameter CameraCBParameter;
			CameraCBParameter.Type = GRootSignature::GE_PARAMETER_CBV;
			CameraCBParameter.ShaderRegisterIndex = 1;
			DepthPipelineState->GetPipelineRootSignature()->AddRootParameter(CameraCBParameter);

			DepthPipelineState->InitializePipelineState();
			PipelineStateList[DEPTH_PSO] = DepthPipelineState;
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

#include "GuardianShaderSystem.h"

namespace GE
{
	const GString GuardianShaderSystem::PBR_MAIN_SHADER = "PBR - Main";
	const GString GuardianShaderSystem::WIREFRAME_SHADER = "Wireframe";
	const GString GuardianShaderSystem::SOLID_SHADER = "Solid";

	std::map<GString, std::shared_ptr<GuardianShaderGroup>> GuardianShaderSystem::ShaderGroupList;


	void GuardianShaderSystem::InitializeShaderSystem()
	{
		{
			std::shared_ptr<GuardianShaderGroup> PBRShaderGroup = std::make_shared<GuardianShaderGroup>();
			PBRShaderGroup->SetGroupName(PBR_MAIN_SHADER);

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			auto& vs = GuardianVertexShader::CreateNewVertexShader("../Guardian Engine/Shaders/PBRShader.gvsh");
			auto& ps = GuardianPixelShader::CreateNewPixelShader("../Guardian Engine/Shaders/PBRShader.gpsh");
			PBRShaderGroup->SetVertexShader(vs, id, ARRAYSIZE(id));
			PBRShaderGroup->SetPixelShader(ps);

			AddShaderGroup(PBRShaderGroup);

			GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, std::format("Loaded engine shader group '{}'", PBR_MAIN_SHADER));
		}

		{
			std::shared_ptr<GuardianShaderGroup> WireframeShaderGroup = std::make_shared<GuardianShaderGroup>();
			WireframeShaderGroup->SetGroupName(WIREFRAME_SHADER);

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			auto& vs = GuardianVertexShader::CreateNewVertexShader("../Guardian Engine/Shaders/Wireframe.gvsh");
			auto& ps = GuardianPixelShader::CreateNewPixelShader("../Guardian Engine/Shaders/Wireframe.gpsh");
			WireframeShaderGroup->SetVertexShader(vs, id, ARRAYSIZE(id));
			WireframeShaderGroup->SetPixelShader(ps);

			AddShaderGroup(WireframeShaderGroup);

			GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, std::format("Loaded engine shader group '{}'", WIREFRAME_SHADER));
		}

		{
			std::shared_ptr<GuardianShaderGroup> SolidShaderGroup = std::make_shared<GuardianShaderGroup>();
			SolidShaderGroup->SetGroupName(SOLID_SHADER);

			D3D11_INPUT_ELEMENT_DESC id[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			auto& vs = GuardianVertexShader::CreateNewVertexShader("../Guardian Engine/Shaders/Solid.gvsh");
			auto& ps = GuardianPixelShader::CreateNewPixelShader("../Guardian Engine/Shaders/Solid.gpsh");
			SolidShaderGroup->SetVertexShader(vs, id, ARRAYSIZE(id));
			SolidShaderGroup->SetPixelShader(ps);

			AddShaderGroup(SolidShaderGroup);

			GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, std::format("Loaded engine shader group '{}'", SOLID_SHADER));
		}

		GUARDIAN_LOG(GuardianMessage::GE_LEVEL_SUCCESS, "Success to initialize shader system.");
	}

	void GuardianShaderSystem::AddShaderGroup(std::shared_ptr<GuardianShaderGroup> shaderGroup)
	{
		if (ShaderGroupList.count(shaderGroup->GetGroupName()) > 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(
				std::format("There has already been a group called : '{}' in shader system!", shaderGroup->GetGroupName()));
		}
		else
		{
			ShaderGroupList[shaderGroup->GetGroupName()] = shaderGroup;
		}
	}

	std::shared_ptr<GuardianShaderGroup> GuardianShaderSystem::GetShaderGroup(const GString& name)
	{
		if (ShaderGroupList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader group found named : '{}' !", name));
		}

		return ShaderGroupList[name];
	}

	std::shared_ptr<GuardianShaderGroup> GuardianShaderSystem::GetShaderGroup(const GuardianUUID& id)
	{
		for (auto& group : ShaderGroupList)
		{
			if (group.second->GetGroupId() == id)
			{
				return group.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No shader group found with id : '{}' !", (unsigned long long)id));
	}
}

#include "GLightRegistry.h"
#include "../DepthMap/GDepthMap.h"
#include "../Applicable/PipelineState/GPipelineStateRegistry.h"

namespace GE
{
	GLightRegistry::GLightRegistry()
	{
		this->PointLightList.clear();

		if (!GShaderViewRegistry::HasShaderViewGroup("PointLightDepthMapGroup"))
		{
			this->PointLightDepthMapGroup = std::make_shared<GShaderViewGroup>("PointLightDepthMapGroup", GLightRegistry::MaxLightCount);
			GShaderViewRegistry::RegistryShaderViewGroup(this->PointLightDepthMapGroup);

			this->PointLightDepthMapGroup->SetRootParameterIndex(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()->
				GetRootParameterIndex(GRootSignature::RootParameter(GRootSignature::GE_PARAMETER_SRV, 5, GLightRegistry::MaxLightCount)));
		}
		else
		{
			this->PointLightDepthMapGroup = GShaderViewRegistry::GetShaderViewGroup("PointLightDepthMapGroup");
		}
	}

	GLightRegistry::GLightRegistry(const GLightRegistry& other)
	{
		this->PointLightList = other.PointLightList;
		this->PointLightDepthMapGroup = other.PointLightDepthMapGroup;
	}

	void GLightRegistry::RegisterPointLight(GPointLight* pointLight)
	{
		if (this->HasPointLight(pointLight))
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("The point light with id : '{}' already exists in the registry!",
				(unsigned long long)pointLight->GetPointLightId()));
		}

		this->PointLightList.push_back(pointLight);
		this->PointLightDepthMapGroup->AddGroupMember(pointLight->LightDepthCubeMap.get());
		pointLight->InitializeDepthRendering();

		if (this->PointLightList.size() > GLightRegistry::MaxLightCount)
		{
			throw GUARDIAN_ERROR_EXCEPTION("The point light count is larger than which the engine supports");
		}
	}

	void GLightRegistry::UnregisterPointLight(GPointLight* pointLight)
	{
		for (auto& it = this->PointLightList.begin(); it != this->PointLightList.end(); it++)
		{
			if ((*it)->GetPointLightId() == pointLight->GetPointLightId())
			{
				this->PointLightList.erase(it);

				this->PointLightDepthMapGroup->RemoveGroupMember(pointLight->LightDepthCubeMap.get());
				pointLight->LightDepthCubeMap->GetDepthCubeMapBuffer().Reset();
				pointLight->LightDepthCubeMap->GetViewDescriptorHandle()->CPUHandle.ptr = 0;
				pointLight->LightDepthCubeMap->GetViewDescriptorHandle()->GPUHandle.ptr = 0;
				return;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No point light with id : '{}' is found in the registry!",
			(unsigned long long)pointLight->GetPointLightId()));
	}

	void GLightRegistry::UnregisterPointLight(GUUID id)
	{
		for (auto& it = this->PointLightList.begin(); it != this->PointLightList.end(); it++)
		{
			if ((*it)->GetPointLightId() == id)
			{
				this->PointLightList.erase(it);

				this->PointLightDepthMapGroup->RemoveGroupMember((*it)->LightDepthCubeMap.get());
				(*it)->LightDepthCubeMap->GetDepthCubeMapBuffer().Reset();
				(*it)->LightDepthCubeMap->GetViewDescriptorHandle()->CPUHandle.ptr = 0;
				(*it)->LightDepthCubeMap->GetViewDescriptorHandle()->GPUHandle.ptr = 0;
				return;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No point light with id : '{}' is found in the registry!", (unsigned long long)id));
	}

	bool GLightRegistry::HasPointLight(GPointLight* pointLight)
	{
		for (auto& pl : this->PointLightList)
		{
			if (pl->GetPointLightId() == pointLight->GetPointLightId())
			{
				return true;
			}
		}

		return false;
	}

	bool GLightRegistry::HasPointLight(GUUID id)
	{
		for (auto& pl : this->PointLightList)
		{
			if (pl->GetPointLightId() == id)
			{
				return true;
			}
		}

		return false;
	}

	bool GLightRegistry::HasPointLights()
	{
		return this->PointLightList.empty() != true;
	}

	std::vector<GPointLight*> GLightRegistry::GetPointLightList()
	{
		return this->PointLightList;
	}
}

#include "GMaterial.h"
#include "../PipelineState/GPipelineStateRegistry.h"

namespace GE
{
	GMaterial::GMaterial()
	{
		this->MaterialName = "";

		this->AlbedoTexture = null;
		this->AlbedoValue = {};
		this->EnableAlbedoTexture = false;

		this->RoughnessTexture = null;
		this->RoughnessValue = 0.8f;
		this->EnableRoughnessTexture = false;

		this->MetallicTexture = null;
		this->MetallicValue = 0.0f;
		this->EnableMetallicTexture = false;

		this->AoTexture = null;
		this->AoValue = 0.8f;
		this->EnableAoTexture = false;

		this->NormalTexture = null;
		this->EnableNormalTexture = false;

		this->MaterialCBuffer = null;
	}

	GMaterial::GMaterial(const std::string& name)
	{
		this->InitializeMaterial(name);
	}

	GMaterial::GMaterial(const GMaterial& other)
	{
		this->MaterialId = other.MaterialId;
		this->MaterialName = other.MaterialName;

		this->AlbedoTexture = other.AlbedoTexture;
		this->AlbedoValue = other.AlbedoValue;
		this->EnableAlbedoTexture = other.EnableAlbedoTexture;

		this->RoughnessTexture = other.RoughnessTexture;
		this->RoughnessValue = other.RoughnessValue;
		this->EnableRoughnessTexture;

		this->MetallicTexture = other.MetallicTexture;
		this->MetallicValue = other.MetallicValue;
		this->EnableMetallicTexture = other.EnableMetallicTexture;

		this->AoTexture = other.AoTexture;
		this->AoValue = other.AoValue;
		this->EnableAoTexture = other.EnableAoTexture;

		this->MaterialCBuffer = other.MaterialCBuffer;
	}

	GMaterial::~GMaterial()
	{
		this->MaterialId = 0;
		this->MaterialName.clear();
	}

	void GMaterial::InitializeMaterial(const std::string& name)
	{
		this->MaterialName = name;

		this->MaterialCBuffer = GMaterialCBuffer::CreateNewMaterialCBuffer(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature());

		this->AlbedoTexture = null;
		this->AlbedoValue = {};
		this->EnableAlbedoTexture = false;

		this->RoughnessTexture = null;
		this->RoughnessValue = 0.8f;
		this->EnableRoughnessTexture = false;

		this->MetallicTexture = null;
		this->MetallicValue = 0.0f;
		this->EnableMetallicTexture = false;

		this->AoTexture = null;
		this->AoValue = 0.8f;
		this->EnableAoTexture = false;

		this->NormalTexture = null;
		this->EnableNormalTexture = false;
	}

	void GMaterial::SetAlbedoTexture(std::shared_ptr<GTexture> albedo, bool enableAlbedoTexure)
	{
		this->AlbedoTexture = albedo;

		this->SetAlbedoTextureEnable(enableAlbedoTexure);
	}

	void GMaterial::SetAlbedoValue(const GVector3& albedo, bool disableAlbedoTexture)
	{
		this->AlbedoValue = albedo;

		this->SetAlbedoTextureEnable(!disableAlbedoTexture);
	}

	void GMaterial::SetAlbedoTextureEnable(bool enable)
	{
		this->EnableAlbedoTexture = enable;
	}

	void GMaterial::SetRoughnessTexture(std::shared_ptr<GTexture> roughness, bool enableRoughnessTexture)
	{
		this->RoughnessTexture = roughness;

		this->SetRoughnessTextureEnable(enableRoughnessTexture);
	}

	void GMaterial::SetRoughnessValue(const float& roughness, bool disableRoughnessTexture)
	{
		this->RoughnessValue = roughness;

		this->SetRoughnessTextureEnable(!disableRoughnessTexture);
	}

	void GMaterial::SetRoughnessTextureEnable(bool enable)
	{
		this->EnableRoughnessTexture = enable;
	}

	void GMaterial::SetMetallicTexture(std::shared_ptr<GTexture> metallic, bool enableMetallicTexture)
	{
		this->MetallicTexture = metallic;

		this->SetMetallicTextureEnable(enableMetallicTexture);
	}

	void GMaterial::SetMetallicValue(const float& metallic, bool disableMetallicTexture)
	{
		this->MetallicValue = metallic;

		this->SetMetallicTextureEnable(!disableMetallicTexture);
	}

	void GMaterial::SetMetallicTextureEnable(bool enable)
	{
		this->EnableMetallicTexture = enable;
	}

	void GMaterial::SetAoTexture(std::shared_ptr<GTexture> ao, bool enableAoTexture)
	{
		this->AoTexture = ao;

		this->SetAlbedoTextureEnable(enableAoTexture);
	}

	void GMaterial::SetAoValue(const float& ao, bool disableAoTexture)
	{
		this->AoValue = ao;

		this->SetAoTextureEnable(!disableAoTexture);
	}

	void GMaterial::SetAoTextureEnable(bool enable)
	{
		this->EnableAoTexture = enable;
	}

	void GMaterial::SetNormalTexture(std::shared_ptr<GTexture> normal, bool enableNormalTexture)
	{
		this->NormalTexture = normal;

		this->SetNormalTextureEnable(enableNormalTexture);
	}

	void GMaterial::SetNormalTextureEnable(bool enable)
	{
		this->EnableNormalTexture = enable;
	}

	void GMaterial::Apply()
	{
		GMaterialCBData Data;
		Data.AlbedoValue = this->AlbedoValue;
		Data.RoughnessValue = this->RoughnessValue;
		Data.MetallicValue = this->MetallicValue;
		Data.AoValue = this->AoValue;
		Data.EnableAlbedoMap = this->EnableAlbedoTexture;
		Data.EnableRoughnessMap = this->EnableRoughnessTexture;
		Data.EnableMetallicMap = this->EnableMetallicTexture;
		Data.EnableAoMap = this->EnableAoTexture;
		Data.EnableNormalMap = this->EnableNormalTexture;

		this->MaterialCBuffer->UpdateBufferData(Data);
		this->MaterialCBuffer->Apply();

		if (this->EnableAlbedoTexture)
		{
			this->AlbedoTexture->Apply();
		}
		if (this->EnableRoughnessTexture)
		{
			this->RoughnessTexture->Apply();
		}
		if (this->EnableMetallicTexture)
		{
			this->MetallicTexture->Apply();
		}
		if (this->EnableAoTexture)
		{
			this->AoTexture->Apply();
		}
		if (this->EnableNormalTexture)
		{
			this->NormalTexture->Apply();
		}
	}

	const GUUID& GMaterial::GetMaterialId() const noexcept
	{
		return this->MaterialId;
	}

	const std::string& GMaterial::GetMaterialName() const noexcept
	{
		return this->MaterialName;
	}
}

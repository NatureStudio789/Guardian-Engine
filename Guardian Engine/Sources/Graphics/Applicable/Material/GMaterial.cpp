#include "GMaterial.h"
#include "../PipelineState/GPipelineStateRegistry.h"
#include "../../../Asset/Loader/GAssetLoaderRegistry.h"

namespace GE
{
	GMaterial::GMaterial()
	{
		this->MaterialName = "";

		this->AlbedoTextureAssetName.clear();
		this->AlbedoTexture = null;
		this->AlbedoValue = {};
		this->EnableAlbedoTexture = false;

		this->RoughnessTextureAssetName.clear();
		this->RoughnessTexture = null;
		this->RoughnessValue = 0.8f;
		this->EnableRoughnessTexture = false;

		this->MetallicTextureAssetName.clear();
		this->MetallicTexture = null;
		this->MetallicValue = 0.0f;
		this->EnableMetallicTexture = false;

		this->AoTextureAssetName.clear();
		this->AoTexture = null;
		this->AoValue = 0.8f;
		this->EnableAoTexture = false;

		this->NormalTextureAssetName.clear();
		this->NormalTexture = null;
		this->EnableNormalTexture = false;
	}

	GMaterial::GMaterial(const std::string& name)
	{
		this->InitializeMaterial(name);
	}

	GMaterial::GMaterial(const GMaterial& other)
	{
		this->MaterialId = other.MaterialId;
		this->MaterialName = other.MaterialName;

		this->AlbedoTextureAssetName = other.AlbedoTextureAssetName;
		this->AlbedoTexture = other.AlbedoTexture;
		this->AlbedoValue = other.AlbedoValue;
		this->EnableAlbedoTexture = other.EnableAlbedoTexture;

		this->RoughnessTextureAssetName = other.RoughnessTextureAssetName;
		this->RoughnessTexture = other.RoughnessTexture;
		this->RoughnessValue = other.RoughnessValue;
		this->EnableRoughnessTexture;

		this->MetallicTextureAssetName = other.MetallicTextureAssetName;
		this->MetallicTexture = other.MetallicTexture;
		this->MetallicValue = other.MetallicValue;
		this->EnableMetallicTexture = other.EnableMetallicTexture;

		this->AoTextureAssetName = other.AoTextureAssetName;
		this->AoTexture = other.AoTexture;
		this->AoValue = other.AoValue;
		this->EnableAoTexture = other.EnableAoTexture;

		this->NormalTextureAssetName = other.NormalTextureAssetName;
		this->NormalTexture = other.NormalTexture;
		this->EnableNormalTexture = other.EnableNormalTexture;

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

		this->AlbedoTextureAssetName.clear();
		this->AlbedoTexture = null;
		this->AlbedoValue = {};
		this->EnableAlbedoTexture = false;

		this->RoughnessTextureAssetName.clear();
		this->RoughnessTexture = null;
		this->RoughnessValue = 0.8f;
		this->EnableRoughnessTexture = false;

		this->MetallicTextureAssetName.clear();
		this->MetallicTexture = null;
		this->MetallicValue = 0.0f;
		this->EnableMetallicTexture = false;

		this->AoTextureAssetName.clear();
		this->AoTexture = null;
		this->AoValue = 0.8f;
		this->EnableAoTexture = false;

		this->NormalTextureAssetName.clear();
		this->NormalTexture = null;
		this->EnableNormalTexture = false;
	}

	void GMaterial::SetAlbedoTexture(const std::string& albedoAssetName, bool enableAlbedoTexure)
	{
		this->AlbedoTextureAssetName = albedoAssetName;
		if (this->AlbedoTexture)
		{
			this->AlbedoTexture.reset();
			this->AlbedoTexture = null;
		}

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

	void GMaterial::SetRoughnessTexture(const std::string& roughnessAssetName, bool enableRoughnessTexture)
	{
		this->RoughnessTextureAssetName = roughnessAssetName;
		if (this->RoughnessTexture)
		{
			this->RoughnessTexture.reset();
			this->RoughnessTexture = null;
		}

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

	void GMaterial::SetMetallicTexture(const std::string& metallicAssetName, bool enableMetallicTexture)
	{
		this->MetallicTextureAssetName = metallicAssetName;
		if (this->MetallicTexture)
		{
			this->MetallicTexture.reset();
			this->MetallicTexture = null;
		}

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

	void GMaterial::SetAoTexture(const std::string& aoAssetName, bool enableAoTexture)
	{
		this->AoTextureAssetName = aoAssetName;
		if (this->AoTexture)
		{
			this->AoTexture.reset();
			this->AoTexture = null;
		}

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

	void GMaterial::SetNormalTexture(const std::string& normalAssetName, bool enableNormalTexture)
	{
		this->NormalTextureAssetName = normalAssetName;
		if (this->NormalTexture)
		{
			this->NormalTexture.reset();
			this->NormalTexture = null;
		}

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
			if (!this->AlbedoTexture)
			{
				this->AlbedoTexture = GTexture::CreateNewTexture(
					GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
					*GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(this->AlbedoTextureAssetName)->GetAssetData<std::shared_ptr<GSurface>>().get(), 0);
			}

			this->AlbedoTexture->Apply();
		}
		if (this->EnableRoughnessTexture)
		{
			if (!this->RoughnessTexture)
			{
				this->RoughnessTexture = GTexture::CreateNewTexture(
					GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
					*GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(this->RoughnessTextureAssetName)->GetAssetData<std::shared_ptr<GSurface>>().get(), 1);
			}

			this->RoughnessTexture->Apply();
		}
		if (this->EnableMetallicTexture)
		{
			if (!this->MetallicTexture)
			{
				this->MetallicTexture = GTexture::CreateNewTexture(
					GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
					*GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(this->MetallicTextureAssetName)->GetAssetData<std::shared_ptr<GSurface>>().get(), 2);
			}

			this->MetallicTexture->Apply();
		}
		if (this->EnableAoTexture)
		{
			if (!this->AoTexture)
			{
				this->AoTexture = GTexture::CreateNewTexture(
					GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
					*GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(this->AoTextureAssetName)->GetAssetData<std::shared_ptr<GSurface>>().get(), 3);
			}

			this->AoTexture->Apply();
		}
		if (this->EnableNormalTexture)
		{
			if (!this->NormalTexture)
			{
				this->NormalTexture = GTexture::CreateNewTexture(
					GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature(),
					*GAssetLoaderRegistry::GetCurrentAssetLoader()->GetAsset(this->NormalTextureAssetName)->GetAssetData<std::shared_ptr<GSurface>>().get(), 4);
			}

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

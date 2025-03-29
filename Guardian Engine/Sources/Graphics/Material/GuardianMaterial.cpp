#include "GuardianMaterial.h"

namespace GE
{
	GuardianMaterial::GuardianMaterial()
	{
		this->MaterialId = GuardianUUID();

		this->AlbedoTexture = null;
		this->UsingAlbedoTexture = false;

		this->RoughnessTexture = null;
		this->UsingRoughnessTexture = false;

		this->MetallicTexture = null;
		this->UsingMetallicTexture = false;

		this->NormalTexture = null;
		this->UsingNormalTexture = false;

		this->AoTexture = null;
		this->UsingAoTexture = false;

		this->AlbedoColor = { 1.0f, 1.0f, 1.0f };
		this->RoughnessColor = 0.5f;
		this->MetallicColor = 0.5f;
		this->AoColor = 0.8f;
		this->MaterialConstantBuffer = null;
	}

	GuardianMaterial::GuardianMaterial(const GuardianMaterial& other)
	{
		this->MaterialId = other.MaterialId;

		this->AlbedoTexture = other.AlbedoTexture;
		this->UsingAlbedoTexture = other.UsingAlbedoTexture;

		this->RoughnessTexture = other.RoughnessTexture;
		this->UsingRoughnessTexture = other.UsingRoughnessTexture;

		this->MetallicTexture = other.MetallicTexture;
		this->UsingMetallicTexture = other.UsingMetallicTexture;

		this->NormalTexture = other.NormalTexture;
		this->UsingNormalTexture = other.UsingNormalTexture;

		this->AoTexture = other.AoTexture;
		this->UsingAoTexture = other.UsingAoTexture;

		this->AlbedoColor = other.AlbedoColor;
		this->RoughnessColor = other.RoughnessColor;
		this->MetallicColor = other.MetallicColor;
		this->AoColor = other.AoColor;
		this->MaterialConstantBuffer = other.MaterialConstantBuffer;
	}

	GuardianMaterial::~GuardianMaterial()
	{
		this->MaterialId = 0;
	}

	void GuardianMaterial::SetAlbedoTexture(std::shared_ptr<GuardianTexture> albedo)
	{
		this->AlbedoTexture = albedo;
		if (!this->AlbedoTexture)
		{
			this->UsingAlbedoTexture = false;
		}

		this->UsingAlbedoTexture = true;
		this->AlbedoTexture->SetTextureAppliedSlot(0);
	}

	void GuardianMaterial::SetAlbedoColor(const GVector3& albedo)
	{
		this->UsingAlbedoTexture = false;
		this->AlbedoColor = albedo;
	}

	void GuardianMaterial::SetMetallicTexture(std::shared_ptr<GuardianTexture> metallic)
	{
		this->MetallicTexture = metallic;
		if (!this->MetallicTexture)
		{
			this->UsingMetallicTexture = false;
		}

		this->UsingMetallicTexture = true;
		this->MetallicTexture->SetTextureAppliedSlot(1);
	}

	void GuardianMaterial::SetMetallicColor(const float metallic)
	{
		this->UsingMetallicTexture = false;
		this->MetallicColor = metallic;
	}

	void GuardianMaterial::SetRoughnessTexture(std::shared_ptr<GuardianTexture> roughness)
	{
		this->RoughnessTexture = roughness;
		if (!this->RoughnessTexture)
		{
			this->UsingRoughnessTexture = false;
		}

		this->UsingRoughnessTexture = true;
		this->RoughnessTexture->SetTextureAppliedSlot(2);
	}

	void GuardianMaterial::SetRoughnessColor(const float roughness)
	{
		this->UsingRoughnessTexture = false;
		this->RoughnessColor = roughness;
	}

	void GuardianMaterial::SetNormalTexture(std::shared_ptr<GuardianTexture> normal)
	{
		this->NormalTexture = normal;
		if (!this->NormalTexture)
		{
			this->UsingNormalTexture = false;
		}

		this->UsingNormalTexture = true;
		this->NormalTexture->SetTextureAppliedSlot(3);
	}

	void GuardianMaterial::SetAoTexture(std::shared_ptr<GuardianTexture> ao)
	{
		this->AoTexture = ao;
		if (!this->AoTexture)
		{
			this->UsingAoTexture = false;
		}

		this->UsingAoTexture = true;
		this->AoTexture->SetTextureAppliedSlot(4);
	}

	void GuardianMaterial::SetAoColor(const float ao)
	{
		this->UsingAoTexture = false;
		this->AoColor = ao;
	}

	void GuardianMaterial::ApplyMaterial(std::shared_ptr<GuardianGraphics> graphics)
	{
		if (!this->MaterialConstantBuffer)
		{
			this->MaterialConstantBuffer = GuardianMaterialConstantBuffer::CreateNewMaterialConstantBuffer(graphics);
		}

		GuardianMaterialProperties properties;
		properties.AlbedoColor = this->AlbedoColor;
		properties.MetallicColor = this->MetallicColor;
		properties.RoughnessColor = this->RoughnessColor;
		properties.AoColor = this->AoColor;
		properties.UsingAlbedoMap = (int)this->UsingAlbedoTexture;
		properties.UsingMetallicMap = (int)this->UsingMetallicTexture;
		properties.UsingRoughnessMap = (int)this->UsingRoughnessTexture;
		properties.UsingNormalMap = (int)this->UsingNormalTexture;
		properties.UsingAoMap = (int)this->UsingAoTexture;
		this->MaterialConstantBuffer->UpdateData(graphics, properties);

		this->MaterialConstantBuffer->Apply(graphics);

		if (this->UsingAlbedoTexture)
		{
			this->AlbedoTexture->Apply(graphics);
		}
		if (this->UsingMetallicTexture)
		{
			this->MetallicTexture->Apply(graphics);
		}
		if (this->UsingRoughnessTexture)
		{
			this->RoughnessTexture->Apply(graphics);
		}
		if (this->UsingNormalTexture)
		{
			this->NormalTexture->Apply(graphics);
		}
		if (this->UsingAoTexture)
		{
			this->AoTexture->Apply(graphics);
		}
	}

	const GuardianUUID& GuardianMaterial::GetMaterialId() const noexcept
	{
		return this->MaterialId;
	}

	bool GuardianMaterial::operator==(const GuardianMaterial& other) const
	{
		return this->MaterialId == other.MaterialId;
	}
}

#ifndef _GE_GMATERIAL_H_
#define _GE_GMATERIAL_H_
#include "../Texture/GTexture.h"

namespace GE
{
	class GUARDIAN_API GMaterial : public GApplicable
	{
	public:
		GMaterial();
		GMaterial(const std::string& name);
		GMaterial(const GMaterial& other);
		~GMaterial() override;

		void InitializeMaterial(const std::string& name);

		void SetAlbedoTexture(const std::string& albedoAssetName, bool enableAlbedoTexure = true);
		void SetAlbedoValue(const GVector3& albedo, bool disableAlbedoTexture = true);
		void SetAlbedoTextureEnable(bool enable);

		void SetRoughnessTexture(const std::string& roughnessAssetName, bool enableRoughnessTexture = true);
		void SetRoughnessValue(const float& roughness, bool disableRoughnessTexture = true);
		void SetRoughnessTextureEnable(bool enable);

		void SetMetallicTexture(const std::string& metallicAssetName, bool enableMetallicTexture = true);
		void SetMetallicValue(const float& metallic, bool disableMetallicTexture = true);
		void SetMetallicTextureEnable(bool enable);

		void SetAoTexture(const std::string& aoAssetName, bool enableAoTexture = true);
		void SetAoValue(const float& ao, bool disableAoTexture = true);
		void SetAoTextureEnable(bool enable);

		void SetNormalTexture(const std::string& normalAssetName, bool enableNormalTexture = true);
		void SetNormalTextureEnable(bool enable);

		void Apply() override;

		const GUUID& GetMaterialId() const noexcept;
		const std::string& GetMaterialName() const noexcept;

		static std::shared_ptr<GMaterial> CreateNewMaterial(const std::string& name)
		{
			return std::make_shared<GMaterial>(name);
		}

	private:
		GUUID MaterialId = GUUID();
		std::string MaterialName;

		std::string AlbedoTextureAssetName;
		std::shared_ptr<GTexture> AlbedoTexture;
		GVector3 AlbedoValue;
		bool EnableAlbedoTexture;

		std::string RoughnessTextureAssetName;
		std::shared_ptr<GTexture> RoughnessTexture;
		float RoughnessValue;
		bool EnableRoughnessTexture;

		std::string MetallicTextureAssetName;
		std::shared_ptr<GTexture> MetallicTexture;
		float MetallicValue;
		bool EnableMetallicTexture;

		std::string AoTextureAssetName;
		std::shared_ptr<GTexture> AoTexture;
		float AoValue;
		bool EnableAoTexture;

		std::string NormalTextureAssetName;
		std::shared_ptr<GTexture> NormalTexture;
		bool EnableNormalTexture;

		std::shared_ptr<GMaterialCBuffer> MaterialCBuffer;
	};
}

#endif
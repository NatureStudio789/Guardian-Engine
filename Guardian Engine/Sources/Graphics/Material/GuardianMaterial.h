#ifndef _GE_GUARDIANMATERIAL_H_
#define _GE_GUARDIANMATERIAL_H_
#include "../ShaderSystem/GuardianShaderSystem.h"

namespace GE
{
	class GUARDIAN_API GuardianMaterial
	{
	public:
		GuardianMaterial();
		GuardianMaterial(const GuardianMaterial& other);
		~GuardianMaterial();

		void SetAlbedoTexture(std::shared_ptr<GuardianTexture> albedo);
		void SetAlbedoColor(const GVector3& albedo);

		void SetMetallicTexture(std::shared_ptr<GuardianTexture> metallic);
		void SetMetallicColor(const float metallic);

		void SetRoughnessTexture(std::shared_ptr<GuardianTexture> roughness);
		void SetRoughnessColor(const float roughness);

		void SetNormalTexture(std::shared_ptr<GuardianTexture> normal);

		void SetAoTexture(std::shared_ptr<GuardianTexture> ao);
		void SetAoColor(const float ao);

		void ApplyMaterial();

		const GuardianUUID& GetMaterialId() const noexcept;

		bool operator==(const GuardianMaterial& other) const;
		
	private:
		GuardianUUID MaterialId;

		std::shared_ptr<GuardianTexture> AlbedoTexture;
		bool UsingAlbedoTexture;

		std::shared_ptr<GuardianTexture> MetallicTexture;
		bool UsingMetallicTexture;

		std::shared_ptr<GuardianTexture> RoughnessTexture;
		bool UsingRoughnessTexture;

		std::shared_ptr<GuardianTexture> NormalTexture;
		bool UsingNormalTexture;

		std::shared_ptr<GuardianTexture> AoTexture;
		bool UsingAoTexture;

		GVector3 AlbedoColor;
		float MetallicColor;
		float RoughnessColor;
		float AoColor;
		std::shared_ptr<GuardianMaterialConstantBuffer> MaterialConstantBuffer;

		friend class GuardianModelImporter;
		friend class GuardianAssetSystem;
		friend class GuardianSceneHierarchyPanel;
	};
}

#endif
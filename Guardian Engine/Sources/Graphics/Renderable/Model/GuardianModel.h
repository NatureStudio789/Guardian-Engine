#ifndef _GE_GUARDIANMODEL_H_
#define _GE_GUARDIANMODEL_H_
#include "../Mesh/GuardianMesh.h"

namespace guardian
{
	class GUARDIAN_API GuardianRenderer;

	class GUARDIAN_API GuardianModel
	{
	public:
		GuardianModel();
		GuardianModel(
			std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);
		GuardianModel(const GuardianModel& other);
		GuardianModel(GuardianModel& other);
		~GuardianModel();

		void InitializeModel(
			std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);
		void SubmitToRenderer();

		void UpdateModel(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
		void UpdateModelLighting(GuardianLightProperties properties);

		void ClearModelMeshList();

		const GString& GetModelFilePath() const noexcept;
		const std::map<GString, std::shared_ptr<GuardianMesh>>& GetModelMeshList() const noexcept;
		const std::map<GString, std::shared_ptr<GuardianMaterial>>& GetModelMaterialList() const noexcept;

		static std::shared_ptr<GuardianModel> CreateNewModel(
			std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);

	private:
		void ProcessModelNode(
			std::shared_ptr<GuardianGraphics> graphics, aiNode* node, const aiScene* scene);
		std::shared_ptr<GuardianMesh> ProcessModelMesh(
			std::shared_ptr<GuardianGraphics> graphics, aiMesh* mesh, const aiScene* scene);
		std::shared_ptr<GuardianTexture> LoadMaterialTexture(
			std::shared_ptr<GuardianGraphics> graphics, aiMaterial* material, aiTextureType type);

		GString ModelFileDirectory;
		GString ModelFilePath;
		std::map<GString, std::shared_ptr<GuardianMesh>> ModelMeshList;
		std::map<GString, std::shared_ptr<GuardianMaterial>> ModelMaterialList;
	};
}

#endif
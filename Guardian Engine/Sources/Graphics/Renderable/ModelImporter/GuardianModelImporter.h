#ifndef _GE_GUARDIANMODELIMPORTER_H_
#define _GE_GUARDIANMODELIMPORTER_H_
#include "../Mesh/GuardianMeshInstance.h"

namespace GE
{
	class GUARDIAN_API GuardianModelImporter
	{
	public:
		GuardianModelImporter();
		GuardianModelImporter(
			std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);
		GuardianModelImporter(const GuardianModelImporter& other);
		GuardianModelImporter(GuardianModelImporter& other);
		~GuardianModelImporter();

		void ImportModel(std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);

		void ClearModelMeshList();

		const GString& GetModelFilePath() const noexcept;
		const std::vector<GuardianMeshInstance::Data>& GetModelMeshInstanceDataList() const noexcept;

		static std::shared_ptr<GuardianModelImporter> CreateNewModel(
			std::shared_ptr<GuardianGraphics> graphics, const GString& modelFilePath);

	private:
		void ProcessModelNode(
			std::shared_ptr<GuardianGraphics> graphics, aiNode* node, const aiScene* scene);
		GuardianMeshInstance::Data ProcessModelMeshInstanceData(
			std::shared_ptr<GuardianGraphics> graphics, aiMesh* mesh, const aiScene* scene);
		std::shared_ptr<GuardianTexture> LoadMaterialTexture(
			std::shared_ptr<GuardianGraphics> graphics, aiMaterial* material, aiTextureType type);
		std::shared_ptr<GuardianTexture> LoadMetallicTexture(
			std::shared_ptr<GuardianGraphics> graphics, aiMaterial* material);

		GString ModelFileDirectory;
		GString ModelFilePath;
		std::vector<GuardianMeshInstance::Data> ModelMeshInstanceDataList;
	};
}

#endif
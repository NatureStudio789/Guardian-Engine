#ifndef _GE_GUARDIANMODELIMPORTER_H_
#define _GE_GUARDIANMODELIMPORTER_H_
#include "../Mesh/GuardianMeshInstance.h"

namespace GE
{
	class GUARDIAN_API GuardianModelImporter
	{
	public:
		GuardianModelImporter();
		GuardianModelImporter(const GString& modelFilePath);
		GuardianModelImporter(const GuardianModelImporter& other);
		GuardianModelImporter(GuardianModelImporter& other);
		~GuardianModelImporter();

		void ImportModel(const GString& modelFilePath);

		void ClearModelMeshList();

		const GString& GetModelFilePath() const noexcept;
		const std::vector<GuardianMeshInstance::Data>& GetModelMeshInstanceDataList() const noexcept;

		static std::shared_ptr<GuardianModelImporter> CreateNewModel(const GString& modelFilePath);

	private:
		void ProcessModelNode(aiNode* node, const aiScene* scene);
		GuardianMeshInstance::Data ProcessModelMeshInstanceData(aiMesh* mesh, const aiScene* scene);
		std::shared_ptr<GuardianTexture> LoadMaterialTexture(aiMaterial* material, aiTextureType type);
		std::shared_ptr<GuardianTexture> LoadMetallicTexture(aiMaterial* material);

		GString ModelFileDirectory;
		GString ModelFilePath;
		std::vector<GuardianMeshInstance::Data> ModelMeshInstanceDataList;
	};
}

#endif
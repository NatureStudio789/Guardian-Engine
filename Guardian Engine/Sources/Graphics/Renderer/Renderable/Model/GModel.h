#ifndef _GE_GMODEL_H_
#define _GE_GMODEL_H_
#include "../Mesh/GMesh.h"

namespace GE
{
	class GUARDIAN_API GModel
	{
	public:
		GModel();
		GModel(const std::string& filePath);
		GModel(const GModel& other);
		~GModel();

		void InitializeModel(const std::string& filePath);

		void SetTransform(const GTransform& transform);
		void Submit(const std::string& channel);

		void LinkTechnique(std::string renderGraphName);

		std::shared_ptr<GMeshNode> GetRootMeshNode();

		static std::shared_ptr<GModel> CreateNewModel(const std::string& filePath)
		{
			return std::make_shared<GModel>(filePath);
		}

	private:
		std::shared_ptr<GMesh> ParseMesh(const aiScene* scene, aiMesh* mesh);
		std::shared_ptr<GMeshNode> ParseNode(const aiScene* scene, aiNode* node);
		std::shared_ptr<GTexture> LoadTexture(aiMaterial* material, aiTextureType type, int index);

		std::shared_ptr<GMeshNode> RootMeshNode;
		std::vector<std::shared_ptr<GMesh>> ModelMeshList;

		std::string ModelFilePath;
		std::string ModelFileDirectory;
	};
}

#endif
#ifndef _GE_GMODEL_H_
#define _GE_GMODEL_H_
#include "../Mesh/GMesh.h"

namespace GE
{
	class GUARDIAN_API GModel
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const Data&) = default;

			std::string FilePath;
			std::string FileDirectory;
			std::vector<GMesh::Data> MeshDataList;
			GMeshNode::Data RootMeshNodeData;
		};

	public:
		GModel();
		GModel(const std::string& filePath, std::string renderGraphName = "Scene");
		GModel(const Data& data, std::string renderGraphName = "Scene");
		GModel(const GModel& other);
		~GModel();

		void InitializeModel(const std::string& filePath, std::string renderGraphName = "Scene");
		void InitializeModel(const Data& data, std::string renderGraphName = "Scene");

		void SetTransform(const GTransform& transform);
		void SetAccumulatedMatrix(const GMatrix& accumulatedMatrix);
		void Submit(const std::string& channel);

		std::shared_ptr<GMeshNode> GetRootMeshNode();

		static std::shared_ptr<GModel> CreateNewModel(const std::string& filePath, std::string renderGraphName = "Scene")
		{
			return std::make_shared<GModel>(filePath, renderGraphName);
		}
		static std::shared_ptr<GModel> CreateNewModel(const Data& data, std::string renderGraphName = "Scene")
		{
			return std::make_shared<GModel>(data, renderGraphName);
		}

		static Data Load(const std::string& filePath)
		{
			Data ModelData;
			ModelData.FilePath = filePath;
			ModelData.FileDirectory = GUtil::GetFilePathDirectory(filePath);

			Assimp::Importer Importer;
			const aiScene* Scene = Importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				throw GUARDIAN_ERROR_EXCEPTION(Importer.GetErrorString());
			}

			std::vector<GMesh::Data> MeshDataList;
			for (UINT i = 0; i < Scene->mNumMeshes; i++)
			{
				MeshDataList.push_back(ParseMesh(Scene, Scene->mMeshes[i], GUtil::GetFilePathDirectory(filePath)));
			}

			ModelData.MeshDataList = MeshDataList;

			ModelData.RootMeshNodeData = ParseNode(Scene, Scene->mRootNode, MeshDataList);

			return ModelData;
		}

	private:
		void LinkTechnique(std::string renderGraphName);
		std::shared_ptr<GMeshNode> BuildMeshNode(GMeshNode::Data meshNodeData);

		static GMesh::Data ParseMesh(const aiScene* scene, aiMesh* mesh, std::string modelFileDirectory);
		static GMeshNode::Data ParseNode(const aiScene* scene, aiNode* node, std::vector<GMesh::Data> meshDataList);
		static std::shared_ptr<GTexture> LoadTexture(aiMaterial* material, aiTextureType type, int index, std::string modelFileDirectory);
		static std::shared_ptr<GTexture> LoadMetallicTexture(aiMaterial* material, int index, std::string modelFileDirectory);

		std::shared_ptr<GMeshNode> RootMeshNode;
		std::vector<std::shared_ptr<GMesh>> ModelMeshList;
		GMatrix ModelAccumulatedMatrix;

		std::string ModelFilePath;
		std::string ModelFileDirectory;
	};
}

#endif
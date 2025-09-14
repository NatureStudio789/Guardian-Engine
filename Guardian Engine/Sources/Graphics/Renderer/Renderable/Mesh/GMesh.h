#ifndef _GE_GMESH_H_
#define _GE_GMESH_H_
#include "../GRenderable.h"

namespace GE
{
	class GUARDIAN_API GMesh : public GRenderable
	{
	public:
		struct Vertex
		{
			GVector3 Position;
			GVector2 TextureCoord;
			GVector3 Normal;
		};

		struct Data
		{
			Data() = default;
			Data(const Data& other)
			{
				this->MeshName = other.MeshName;
				this->Vertices = other.Vertices;
				this->Indices = other.Indices;
				this->MeshMaterialId = other.MeshMaterialId;
			}

			std::string MeshName;
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
			GUUID MeshMaterialId;
		};

	public:
		GMesh();
		GMesh(const Data& data);
		GMesh(const GMesh& other);
		~GMesh() override;

		void InitializeMesh(const Data& data);

		const Data& GetMeshData() const noexcept;

		static std::shared_ptr<GMesh> CreateNewMesh(const Data& data)
		{
			return std::make_shared<GMesh>(data);
		}

	private:
		Data MeshData;

		friend class GMeshNode;
	};

	class GUARDIAN_API GMeshNode
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const Data&) = default;

			std::string NodeName;
			std::vector<UINT> MeshDataIndexList;
			GMatrix ExtraMatrix;

			std::vector<Data> ChildrenList;
		};

	public:
		GMeshNode();
		GMeshNode(const std::string& nodeName, std::vector<std::shared_ptr<GMesh>> meshList);
		GMeshNode(const GMeshNode& other);
		~GMeshNode();

		void SetMeshNodeName(const std::string& nodeName);
		void SetMeshList(std::vector<std::shared_ptr<GMesh>> meshList);
		void AddMesh(std::shared_ptr<GMesh> mesh);

		void SetTransform(const GTransform& transform);
		void Submit(const std::string& channel, GMatrix accumulatedMatrix);

		const GUUID& GetNodeId() const noexcept;
		const std::string& GetNodeName() const noexcept;
		const bool HasChildren() const noexcept;
		std::shared_ptr<GMeshNode> GetChild(const std::string& childName);

	private:
		void AddChild(std::shared_ptr<GMeshNode> node);
		void SetExtraMatrix(const GMatrix& matrix);

		GUUID NodeId = GUUID();
		std::string NodeName;
		
		std::vector<std::shared_ptr<GMeshNode>> ChildrenList;
		std::vector<std::shared_ptr<GMesh>> MeshList;

		GTransform Transform;
		GMatrix ExtraMatrix;

		friend class GModel;
	};
}

#endif
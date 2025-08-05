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
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
		};

	public:
		GMesh();
		GMesh(const std::string& name, const Data& data, std::shared_ptr<GMaterial> material);
		GMesh(const GMesh& other);
		~GMesh() override;

		void InitializeMesh(const std::string& name, const Data& data, std::shared_ptr<GMaterial> material);

		static std::shared_ptr<GMesh> CreateNewMesh(const std::string& name, const Data& data, std::shared_ptr<GMaterial> material)
		{
			return std::make_shared<GMesh>(name, data, material);
		}

	private:
		Data MeshData;
		std::shared_ptr<GMaterial> MeshMaterial;
	};

	class GUARDIAN_API GMeshNode
	{
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
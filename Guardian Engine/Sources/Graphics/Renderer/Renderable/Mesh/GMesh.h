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
		};

		struct Data
		{
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
		};

	public:
		GMesh();
		GMesh(const std::string& name, const Data& data);
		GMesh(const GMesh& other);
		~GMesh() override;

		void InitializeMesh(const std::string& name, const Data& data);

		static std::shared_ptr<GMesh> CreateNewMesh(const std::string& name, const Data& data)
		{
			return std::make_shared<GMesh>(name, data);
		}

	private:
		Data MeshData;
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
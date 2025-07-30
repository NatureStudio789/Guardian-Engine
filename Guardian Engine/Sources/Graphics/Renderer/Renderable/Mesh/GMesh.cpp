#include "GMesh.h"

namespace GE
{
	GMesh::GMesh() : GRenderable()
	{
		this->MeshData = {};
	}

	GMesh::GMesh(const std::string& name, const Data& data)
	{
		this->InitializeMesh(name, data);
	}

	GMesh::GMesh(const GMesh& other) : GRenderable(other)
	{
		this->MeshData = other.MeshData;
	}

	GMesh::~GMesh()
	{
		this->MeshData.Vertices.clear();
		this->MeshData.Indices.clear();
	}

	void GMesh::InitializeMesh(const std::string& name, const Data& data)
	{
		this->MeshData = data;

		this->InitializeRenderable(name,
			GVertexBuffer::CreateNewVertexBuffer((void*)this->MeshData.Vertices.data(), (UINT)this->MeshData.Vertices.size(), (UINT)sizeof(Vertex)),
			GIndexBuffer::CreateNewIndexBuffer(this->MeshData.Indices.data(), (UINT)this->MeshData.Indices.size()),
			GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

		auto& tech = GTechnique::CreateNewTechnique("PBR", "main");
		auto& step = GStep::CreateNewStep("Lighting");
		step->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
			GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));

		tech->AddStep(step);
		this->AddTechnique(tech);
	}

	GMeshNode::GMeshNode()
	{
		this->NodeName = "";
		this->ChildrenList.clear();
		this->MeshList.clear();
		this->Transform = {};
	}

	GMeshNode::GMeshNode(const std::string& nodeName, std::vector<std::shared_ptr<GMesh>> meshList)
	{
		this->SetMeshNodeName(nodeName);
		this->SetMeshList(meshList);
	}

	GMeshNode::GMeshNode(const GMeshNode& other)
	{
		this->NodeId = other.NodeId;
		this->NodeName = other.NodeName;
		this->ChildrenList = other.ChildrenList;
		this->MeshList = other.MeshList;
		this->Transform = other.Transform;
	}

	GMeshNode::~GMeshNode()
	{
		this->NodeId = 0;
		this->NodeName.clear();

		for (auto& child : this->ChildrenList)
		{
			child.reset();
			child = null;
		}
		this->ChildrenList.clear();

		for (auto& mesh : this->MeshList)
		{
			mesh.reset();
			mesh = null;
		}
		this->MeshList.clear();

		this->Transform = {};
	}

	void GMeshNode::SetMeshNodeName(const std::string& nodeName)
	{
		this->NodeName = nodeName;
	}

	void GMeshNode::SetMeshList(std::vector<std::shared_ptr<GMesh>> meshList)
	{
		this->MeshList = meshList;
	}

	void GMeshNode::AddMesh(std::shared_ptr<GMesh> mesh)
	{
		this->MeshList.push_back(mesh);
	}

	void GMeshNode::SetTransform(const GTransform& transform)
	{
		this->Transform = transform;
	}

	void GMeshNode::Submit(const std::string& channel, GMatrix accumulatedMatrix)
	{
		const auto Built = this->Transform.GetTransformMatrix() * this->ExtraMatrix * accumulatedMatrix;

		for (auto& mesh : this->MeshList)
		{
			mesh->SetTransform(this->Transform);
			mesh->SetExtraMatrix(this->ExtraMatrix);
			mesh->SetAccumulatedMatrix(accumulatedMatrix);

			mesh->Submit(channel);
		}

		for (auto& child : this->ChildrenList)
		{
			child->Submit(channel, Built);
		}
	}

	const GUUID& GMeshNode::GetNodeId() const noexcept
	{
		return this->NodeId;
	}

	const std::string& GMeshNode::GetNodeName() const noexcept
	{
		return this->NodeName;
	}

	const bool GMeshNode::HasChildren() const noexcept
	{
		return this->ChildrenList.empty() != true;
	}

	void GMeshNode::AddChild(std::shared_ptr<GMeshNode> node)
	{
		this->ChildrenList.push_back(node);
	}

	void GMeshNode::SetExtraMatrix(const GMatrix& matrix)
	{
		this->ExtraMatrix = matrix;
	}
}

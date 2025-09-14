#include "GMesh.h"

namespace GE
{
	GMesh::GMesh() : GRenderable()
	{
		this->MeshData = {};
	}

	GMesh::GMesh(const Data& data)
	{
		this->InitializeMesh(data);
	}

	GMesh::GMesh(const GMesh& other) : GRenderable(other)
	{
		this->MeshData = other.MeshData;
	}

	GMesh::~GMesh()
	{
		this->MeshData.Vertices.clear();
		this->MeshData.Indices.clear();

		this->MeshData.MeshMaterialId = 0;
	}

	void GMesh::InitializeMesh(const Data& data)
	{
		this->MeshData = data;

		this->InitializeRenderable(this->MeshData.MeshName,
			GVertexBuffer::CreateNewVertexBuffer((void*)this->MeshData.Vertices.data(), (UINT)this->MeshData.Vertices.size(), (UINT)sizeof(Vertex)),
			GIndexBuffer::CreateNewIndexBuffer(this->MeshData.Indices.data(), (UINT)this->MeshData.Indices.size()),
			GTopology::CreateNewTopology(GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST));

		auto& LightingTechnique = GTechnique::CreateNewTechnique("Lighting", "main");

		{
			auto& EditRenderStep = GStep::CreateNewStep("Edit");

			EditRenderStep->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));
			EditRenderStep->AddApplicable(GMaterialRegistry::GetMaterial(this->MeshData.MeshMaterialId));

			LightingTechnique->AddStep(EditRenderStep);
		}
		
		{
			auto& RuntimeRenderStep = GStep::CreateNewStep("Runtime");

			RuntimeRenderStep->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));
			RuntimeRenderStep->AddApplicable(GMaterialRegistry::GetMaterial(this->MeshData.MeshMaterialId));

			LightingTechnique->AddStep(RuntimeRenderStep);
		}

		this->AddTechnique(LightingTechnique);

		auto& DebugTechnique = GTechnique::CreateNewTechnique("Debug", "main", false);

		{
			auto& WireframeStep = GStep::CreateNewStep("Wireframe");

			WireframeStep->AddApplicable(GTransformCBuffer::CreateNewTransformCBuffer(
				GPipelineStateRegistry::GetPipelineState(GPipelineStateRegistry::LIGHTING_PSO)->GetPipelineRootSignature()));

			DebugTechnique->AddStep(WireframeStep);
		}

		this->AddTechnique(DebugTechnique);

	}

	const GMesh::Data& GMesh::GetMeshData() const noexcept
	{
		return this->MeshData;
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
		const auto World = this->Transform.GetTransformMatrix();
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

	std::shared_ptr<GMeshNode> GMeshNode::GetChild(const std::string& childName)
	{
		for (auto& child : this->ChildrenList)
		{
			if (child->GetNodeName() == childName)
			{
				return child;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No child mesh node named '{}' found in node named '{}'", childName, this->NodeName));
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

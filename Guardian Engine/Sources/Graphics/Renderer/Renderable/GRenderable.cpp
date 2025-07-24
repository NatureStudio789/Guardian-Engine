#include "GRenderable.h"

namespace GE
{
	GRenderable::GRenderable()
	{
		this->RenderableName = "";

		this->VertexBuffer = null;
		this->IndexBuffer = null;
		this->Topology = null;

		this->RenderTechniqueList.clear();
	}

	GRenderable::GRenderable(const GRenderable& other)
	{
		this->RenderableId = other.RenderableId;
		this->RenderableName = other.RenderableName;

		this->VertexBuffer = other.VertexBuffer;
		this->IndexBuffer = other.IndexBuffer;
		this->Topology = other.Topology;

		this->RenderTechniqueList = other.RenderTechniqueList;
	}

	GRenderable::~GRenderable()
	{
		this->RenderableId = 0;
		this->RenderableName.clear();

		this->VertexBuffer.reset();
		this->VertexBuffer = null;

		this->IndexBuffer.reset();
		this->IndexBuffer = null;

		this->Topology.reset();
		this->Topology = null;

		for (auto& technology : this->RenderTechniqueList)
		{
			technology.reset();
			technology = null;
		}
		this->RenderTechniqueList.clear();
	}

	void GRenderable::AddTechnology(std::shared_ptr<GTechnique> technology)
	{
		this->RenderTechniqueList.push_back(technology);
	}

	void GRenderable::SetTechnologyList(std::vector<std::shared_ptr<GTechnique>> technologyList)
	{
		this->RenderTechniqueList = technologyList;
	}

	void GRenderable::Apply()
	{
		this->VertexBuffer->Apply();
		
		if (this->IndexBuffer)
		{
			this->IndexBuffer->Apply();
		}

		this->Topology->Apply();
	}

	void GRenderable::Submit(const std::string& channel)
	{
		for (auto& technique : this->RenderTechniqueList)
		{
			technique->Submit(std::make_shared<GRenderable>(*this), channel);
		}
	}

	void GRenderable::LinkTechnique(std::string renderGraphName)
	{
		for (auto& technique : this->RenderTechniqueList)
		{
			technique->Link(renderGraphName);
		}
	}

	const GUUID& GRenderable::GetRenderableId() const noexcept
	{
		return this->RenderableId;
	}

	const std::string& GRenderable::GetRenderableName() const noexcept
	{
		return this->RenderableName;
	}

	const UINT& GRenderable::GetVerticesCount() const
	{
		GUARDIAN_CHECK_POINTER(this->VertexBuffer);

		return this->VertexBuffer->GetVerticesCount();
	}

	const UINT& GRenderable::GetIndicesCount() const
	{
		GUARDIAN_CHECK_POINTER(this->IndexBuffer);

		return this->IndexBuffer->GetIndicesCount();
	}

	const UINT& GRenderable::GetDataSize() const
	{
		GUARDIAN_CHECK_POINTER(this->VertexBuffer);

		if (this->IndexBuffer)
		{
			return this->IndexBuffer->GetDataSize();
		}
		else
		{
			return this->VertexBuffer->GetDataSize();
		}
	}

	const bool GRenderable::HasIndexBuffer() const noexcept
	{
		return this->IndexBuffer != null;
	}
}

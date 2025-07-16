#include "GRenderable.h"

namespace GE
{
	GRenderable::GRenderable()
	{
		this->RenderableId = GUUID();
		this->VertexBuffer = null;
		this->IndexBuffer = null;

		this->ApplicableList.clear();
	}

	GRenderable::GRenderable(const GRenderable& other)
	{
		this->RenderableId = other.RenderableId;
		this->VertexBuffer = other.VertexBuffer;
		this->IndexBuffer = other.IndexBuffer;

		this->ApplicableList = other.ApplicableList;
	}

	GRenderable::~GRenderable()
	{
		this->RenderableId = 0;
		
		this->VertexBuffer.reset();
		this->VertexBuffer = null;

		this->IndexBuffer.reset();
		this->IndexBuffer = null;
		
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
			applicable = null;
		}
		this->ApplicableList.clear();
	}

	void GRenderable::AddApplicable(std::shared_ptr<GApplicable> applicable)
	{
		if (typeid(*applicable) == typeid(GVertexBuffer) || typeid(*applicable) == typeid(GIndexBuffer) 
			|| typeid(*applicable) == typeid(GPipelineState) || typeid(*applicable) == typeid(GTransformCBuffer))
		{
			throw GUARDIAN_ERROR_EXCEPTION("AddApplicable function doesn't support to add vertex or index buffer!");
		}

		this->ApplicableList.push_back(applicable);
	}

	void GRenderable::AddVertexBuffer(std::shared_ptr<GVertexBuffer> vertexBuffer)
	{
		this->VertexBuffer = vertexBuffer;
		this->ApplicableList.push_back(vertexBuffer);
	}

	void GRenderable::AddIndexBuffer(std::shared_ptr<GIndexBuffer> indexBuffer)
	{
		this->IndexBuffer = indexBuffer;
		this->ApplicableList.push_back(indexBuffer);
	}

	void GRenderable::AddTransformCBuffer(std::shared_ptr<GTransformCBuffer> cbuffer)
	{
		this->TransformCBuffer = cbuffer;
		this->ApplicableList.push_back(cbuffer);
	}

	void GRenderable::Render()
	{
		GUARDIAN_CHECK_POINTER(this->VertexBuffer);

		this->GetRenderablePipelineState()->Apply();

		for (auto& staticApplicable : this->GetStaticApplicableList())
		{
			staticApplicable->Apply();
		}

		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}

		if (this->IndexBuffer)
		{
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
				DrawIndexedInstanced(this->IndexBuffer->GetBufferSize(), 1, 0, 0, 0);
		}
		else
		{
			GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
				DrawInstanced(this->VertexBuffer->GetBufferSize(), 1, 0, 0);
		}
	}

	void GRenderable::UpdateTransformData(GTransformCBData data)
	{
		this->TransformCBuffer->UpdateBufferData(data);
	}

	const GUUID& GRenderable::GetRenderableId() const noexcept
	{
		return this->RenderableId;
	}

	std::shared_ptr<GVertexBuffer> GRenderable::GetVertexBuffer()
	{
		return this->VertexBuffer;
	}

	std::shared_ptr<GIndexBuffer> GRenderable::GetIndexBuffer()
	{
		return this->IndexBuffer;
	}
}

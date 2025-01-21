#include "GuardianRenderable.h"
#include "../Renderer/GuardianRenderer.h"

namespace guardian
{
	GuardianRenderable::GuardianRenderable()
	{
		this->RenderingVertexBuffer = std::make_shared<GuardianVertexBuffer>();
		this->RenderingIndexBuffer = std::make_shared<GuardianIndexBuffer>();
		this->RenderingTransformConstantBuffer = std::make_shared<GuardianTransformConstantBuffer>();

		this->ApplicableList.clear();
	}

	GuardianRenderable::GuardianRenderable(const GuardianRenderable& other)
	{
		this->RenderingVertexBuffer = other.RenderingVertexBuffer;
		this->RenderingIndexBuffer = other.RenderingIndexBuffer;
		this->RenderingTransformConstantBuffer = other.RenderingTransformConstantBuffer;
		this->ApplicableList = other.ApplicableList;
	}

	GuardianRenderable::~GuardianRenderable()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
	}

	void GuardianRenderable::AddApplicable(std::shared_ptr<GuardianApplicable> applicable)
	{
		if (typeid(*applicable) == typeid(GuardianVertexBuffer))
		{
			throw GUARDIAN_TYPE_EXCEPTION("VertexBuffer", "the type except the VertexBuffer");
		}
		else if (typeid(*applicable) == typeid(GuardianIndexBuffer))
		{
			throw GUARDIAN_TYPE_EXCEPTION("IndexBuffer", "the type except the IndexBuffer");
		}
		else if (typeid(*applicable) == typeid(GuardianTransformConstantBuffer))
		{
			throw GUARDIAN_TYPE_EXCEPTION("TransformConstantBuffer", "the type except the TransformConstantBuffer");
		}

		this->ApplicableList.push_back(applicable);
	}

	void GuardianRenderable::AddVertexBuffer(std::shared_ptr<GuardianVertexBuffer> vertexBuffer)
	{
		if (!this->RenderingVertexBuffer->GetVertexBufferObject().Get())
		{
			this->RenderingVertexBuffer = vertexBuffer;
			this->ApplicableList.push_back(this->RenderingVertexBuffer);
		}
	}

	void GuardianRenderable::AddIndexBuffer(std::shared_ptr<GuardianIndexBuffer> indexBuffer)
	{
		if (!this->RenderingIndexBuffer->GetIndexBufferObject().Get())
		{
			this->RenderingIndexBuffer = indexBuffer;
			this->ApplicableList.push_back(this->RenderingIndexBuffer);
		}
	}

	void GuardianRenderable::AddTransformConstantBuffer(std::shared_ptr<GuardianTransformConstantBuffer> transformCBuffer)
	{
		if (!this->RenderingTransformConstantBuffer->GetConstantBufferObject().Get())
		{
			this->RenderingTransformConstantBuffer = transformCBuffer;
			this->ApplicableList.push_back(this->RenderingTransformConstantBuffer);
		}
	}

	void GuardianRenderable::SubmitToRenderer(GuardianSubmitPassLevel level)
	{
		GuardianRenderer::SubmitRenderable(level, std::make_shared<GuardianRenderable>(*this));
	}

	void GuardianRenderable::Render(std::shared_ptr<GuardianGraphics> graphics)
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply(graphics);
		}

		if (this->RenderingVertexBuffer->GetVertexBufferObject().Get())
		{
			if (this->RenderingIndexBuffer->GetIndexBufferObject().Get())
			{
				graphics->GetGraphicsDeviceContext()->DrawIndexed(
					(UINT)this->RenderingIndexBuffer->GetIndexBufferData().size(), 0, 0);
			}
			else
			{
				graphics->GetGraphicsDeviceContext()->Draw(this->RenderingVertexBuffer->GetVertexBufferDataSize(), 0);
			}
		}
	}
}

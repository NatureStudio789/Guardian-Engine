#include "GuardianRenderable.h"

namespace GE
{
	GuardianRenderable::GuardianRenderable()
	{
		this->RenderableId = GuardianUUID();
		this->UseSpecialShader = false;
		this->RenderingVertexBuffer = std::make_shared<GuardianVertexBuffer>();
		this->RenderingIndexBuffer = std::make_shared<GuardianIndexBuffer>();
		this->RenderingTransformConstantBuffer = std::make_shared<GuardianTransformConstantBuffer>();
		this->RenderingLightConstantBuffer = std::make_shared<GuardianLightConstantBuffer>();
		this->RenderingMaterial = std::make_shared<GuardianMaterial>();
		this->RenderingTexturesNumber = 0;

		this->ApplicableList.clear();
	}

	GuardianRenderable::GuardianRenderable(const GuardianRenderable& other)
	{
		this->RenderingVertexBuffer = other.RenderingVertexBuffer;
		this->RenderingIndexBuffer = other.RenderingIndexBuffer;
		this->UseSpecialShader = other.UseSpecialShader;
		this->RenderingTransformConstantBuffer = other.RenderingTransformConstantBuffer;
		this->RenderingLightConstantBuffer = other.RenderingLightConstantBuffer;
		this->RenderingMaterial = other.RenderingMaterial;
		this->RenderingTexturesNumber = other.RenderingTexturesNumber;
		this->ApplicableList = other.ApplicableList;
	}

	GuardianRenderable::~GuardianRenderable()
	{
		for (auto& applicable : this->ApplicableList)
		{
			applicable.reset();
		}
		this->RenderingTexturesNumber = 0;
	}

	void GuardianRenderable::EnableSpecialShader()
	{
		this->UseSpecialShader = true;
	}

	void GuardianRenderable::DisableSpecialShader()
	{
		this->UseSpecialShader = false;
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
		else if (typeid(*applicable) == typeid(GuardianVertexShader) || 
			typeid(*applicable) == typeid(GuardianPixelShader) || typeid(*applicable) == typeid(GuardianInputLayout))
		{
			if (!this->UseSpecialShader)
			{
				throw GUARDIAN_TYPE_EXCEPTION("Shader", "the special shader of this renderable is banned!");
			}
		}
		else if (typeid(*applicable) == typeid(GuardianTransformConstantBuffer))
		{
			throw GUARDIAN_TYPE_EXCEPTION("TransformConstantBuffer", "the type except the TransformConstantBuffer");
		}
		else if (typeid(*applicable) == typeid(GuardianLightConstantBuffer))
		{
			throw GUARDIAN_TYPE_EXCEPTION("LightConstantBuffer", "the type except the LightConstantBuffer");
		}
		else if (typeid(*applicable) == typeid(GuardianTexture))
		{
			this->RenderingTexturesNumber++;
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

	void GuardianRenderable::AddLightConstantBuffer(std::shared_ptr<GuardianLightConstantBuffer> lightCBuffer)
	{
		if (!this->RenderingLightConstantBuffer->GetConstantBufferObject().Get())
		{
			this->RenderingLightConstantBuffer = lightCBuffer;
			this->ApplicableList.push_back(this->RenderingLightConstantBuffer);
		}
	}

	void GuardianRenderable::AddLightConstantBufferFromStatic()
	{
		for (auto& staticApplicable : this->GetStaticApplicableList())
		{
			if (const auto p = dynamic_cast<GuardianLightConstantBuffer*>(staticApplicable.get()))
			{
				this->RenderingLightConstantBuffer = std::make_shared<GuardianLightConstantBuffer>(*p);
				return;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION("Failed to find the static light constant buffer!");
	}

	void GuardianRenderable::Render()
	{
		for (auto& staticApplicable : this->GetStaticApplicableList())
		{
			staticApplicable->Apply();
		}

		for (auto& applicable : this->ApplicableList)
		{
			applicable->Apply();
		}

		this->RenderingMaterial->ApplyMaterial();

		if (this->RenderingVertexBuffer->GetVertexBufferObject().Get())
		{
			if (this->RenderingIndexBuffer->GetIndexBufferObject().Get())
			{
				GuardianGraphicsRegistry::GetCurrentGraphics()->GetGraphicsDeviceContext()->DrawIndexed(
					(UINT)this->RenderingIndexBuffer->GetIndexBufferData().size(), 0, 0);
			}
			else
			{
				GuardianGraphicsRegistry::GetCurrentGraphics()->
					GetGraphicsDeviceContext()->Draw(this->RenderingVertexBuffer->GetVertexBufferDataSize(), 0);
			}
		}
	}

	const GuardianUUID& GuardianRenderable::GetRenderableId() const noexcept
	{
		return this->RenderableId;
	}

	std::shared_ptr<GuardianLightConstantBuffer> GuardianRenderable::GetLightConstantBuffer()
	{
		return this->RenderingLightConstantBuffer;;
	}

	std::shared_ptr<GuardianTransformConstantBuffer> GuardianRenderable::GetTransformConstantBuffer()
	{
		return this->RenderingTransformConstantBuffer;
	}

	std::shared_ptr<GuardianVertexBuffer> GuardianRenderable::GetVertexBuffer()
	{
		return this->RenderingVertexBuffer;
	}

	std::shared_ptr<GuardianIndexBuffer> GuardianRenderable::GetIndexBuffer()
	{
		return this->RenderingIndexBuffer;
	}

	const UINT& GuardianRenderable::GetTexturesNumber() const noexcept
	{
		return this->RenderingTexturesNumber;
	}

	const GuardianAABB& GuardianRenderable::GetBoundingBox() const noexcept
	{
		return this->RenderingBoundingBox;
	}
}

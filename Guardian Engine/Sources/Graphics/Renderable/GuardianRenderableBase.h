#ifndef _GE_GUARDIANRENDREABLEBASE_H_
#define _GE_GUARDIANRENDREABLEBASE_H_
#include "GuardianRenderable.h"

namespace GE
{
	template<typename T>
	class GuardianRenderableBase : public GuardianRenderable
	{
	public:
		GuardianRenderableBase();
		GuardianRenderableBase(const GuardianRenderableBase& other);

		void AddStaticApplicable(std::shared_ptr<GuardianApplicable> applicable);

		void AddStaticVertexBuffer(std::shared_ptr<GuardianVertexBuffer> vertexBuffer);
		void AddStaticIndexBuffer(std::shared_ptr<GuardianIndexBuffer> indexBuffer);
		void AddStaticLightConstantBuffer(std::shared_ptr<GuardianLightConstantBuffer> lightConstantBuffer);

		const bool IsStaticApplicablesInitialized() const noexcept;

	protected:
		static std::vector<std::shared_ptr<GuardianApplicable>> StaticApplicableList;

	private:
		std::vector<std::shared_ptr<GuardianApplicable>> GetStaticApplicableList() noexcept override;
	};

	template<typename T>
	std::vector<std::shared_ptr<GuardianApplicable>> GuardianRenderableBase<T>::StaticApplicableList;

	template<typename T>
	inline GuardianRenderableBase<T>::GuardianRenderableBase() : GuardianRenderable()
	{
	}

	template<typename T>
	inline GuardianRenderableBase<T>::GuardianRenderableBase(const GuardianRenderableBase& other) : GuardianRenderable(other)
	{
		this->StaticApplicableList = other.StaticApplicableList;
	}

	template<typename T>
	inline void GuardianRenderableBase<T>::AddStaticApplicable(std::shared_ptr<GuardianApplicable> applicable)
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

		StaticApplicableList.push_back(applicable);
	}

	template<typename T>
	inline void GuardianRenderableBase<T>::AddStaticVertexBuffer(std::shared_ptr<GuardianVertexBuffer> vertexBuffer)
	{
		if (!this->RenderingVertexBuffer->GetVertexBufferObject().Get())
		{
			this->RenderingVertexBuffer = vertexBuffer;
			StaticApplicableList.push_back(this->RenderingVertexBuffer);
		}
	}

	template<typename T>
	inline void GuardianRenderableBase<T>::AddStaticIndexBuffer(std::shared_ptr<GuardianIndexBuffer> indexBuffer)
	{
		if (!this->RenderingIndexBuffer->GetIndexBufferObject().Get())
		{
			this->RenderingIndexBuffer = indexBuffer;
			StaticApplicableList.push_back(this->RenderingIndexBuffer);
		}
	}

	template<typename T>
	inline void GuardianRenderableBase<T>::AddStaticLightConstantBuffer(std::shared_ptr<GuardianLightConstantBuffer> lightConstantBuffer)
	{
		if (!this->RenderingLightConstantBuffer->GetConstantBufferObject().Get())
		{
			this->RenderingLightConstantBuffer = lightConstantBuffer;
			StaticApplicableList.push_back(this->RenderingLightConstantBuffer);
		}
	}

	template<typename T>
	inline const bool GuardianRenderableBase<T>::IsStaticApplicablesInitialized() const noexcept
	{
		return !StaticApplicableList.empty();
	}

	template<typename T>
	inline std::vector<std::shared_ptr<GuardianApplicable>> GuardianRenderableBase<T>::GetStaticApplicableList() noexcept
	{
		return StaticApplicableList;
	}
}

#endif
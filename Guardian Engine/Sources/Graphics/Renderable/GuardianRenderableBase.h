#ifndef _GE_GUARDIANRENDREABLEBASE_H_
#define _GE_GUARDIANRENDREABLEBASE_H_
#include "GuardianRenderable.h"

namespace guardian
{
	template<typename T>
	class GuardianRenderableBase : public GuardianRenderable
	{
	public:
		void AddStaticApplicable(std::shared_ptr<GuardianApplicable> applicable);

		void AddStaticVertexBuffer(std::shared_ptr<GuardianVertexBuffer> vertexBuffer);
		void AddStaticIndexBuffer(std::shared_ptr<GuardianIndexBuffer> indexBuffer);

		const bool IsStaticApplicablesInitialized() const noexcept;

	protected:
		static std::vector<std::shared_ptr<GuardianApplicable>> StaticApplicableList;

	private:
		std::vector<std::shared_ptr<GuardianApplicable>> GetStaticApplicableList() noexcept override;
	};

	template<typename T>
	std::vector<std::shared_ptr<GuardianApplicable>> GuardianRenderableBase<T>::StaticApplicableList;

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
#ifndef _GE_GRENDERALBEBASE_H_
#define _GE_GRENDERALBEBASE_H_
#include "GRenderable.h"

namespace GE
{
	template<typename T>
	class GRenderableBase : public GRenderable
	{
	public:
		GRenderableBase();
		GRenderableBase(const GRenderableBase& other);

		void AddStaticApplicable(std::shared_ptr<GApplicable> applicable);
		void SetRenderableShader(std::shared_ptr<GShader> shader);
		void SetRenderableInputLayout(std::shared_ptr<GInputLayout> inputLayout);
		void SetRenderableTopology(std::shared_ptr<GTopology> topology);

		void EndUpInitializing();

		const bool GetStaticInitialized() const noexcept;

	protected:
		static std::vector<std::shared_ptr<GApplicable>> StaticApplicableList;
		static std::shared_ptr<GPipelineState> RenderablePipelineState;

		std::vector<std::shared_ptr<GApplicable>> GetStaticApplicableList() noexcept override;
		std::shared_ptr<GPipelineState> GetRenderablePipelineState() noexcept override;
	};

	template<typename T>
	std::vector<std::shared_ptr<GApplicable>> GRenderableBase<T>::StaticApplicableList;
	template<typename T>
	std::shared_ptr<GPipelineState> GRenderableBase<T>::RenderablePipelineState = std::make_shared<GPipelineState>();


	template<typename T>
	inline GRenderableBase<T>::GRenderableBase() : GRenderable()
	{

	}

	template<typename T>
	inline GRenderableBase<T>::GRenderableBase(const GRenderableBase& other) : GRenderable(other)
	{
		StaticApplicableList = other.StaticApplicableList;
		RenderablePipelineState = other.RenderablePipelineState;
	}

	template<typename T>
	inline void GRenderableBase<T>::AddStaticApplicable(std::shared_ptr<GApplicable> applicable)
	{
		if (typeid(*applicable) == typeid(GVertexBuffer) || typeid(*applicable) == typeid(GIndexBuffer)
			|| typeid(*applicable) == typeid(GPipelineState) || typeid(*applicable) == typeid(GTopology))
		{
			throw GUARDIAN_ERROR_EXCEPTION("Unsupported applicable type for AddStaticApplicable!");
		}

		StaticApplicableList.push_back(applicable);
	}

	template<typename T>
	inline void GRenderableBase<T>::SetRenderableShader(std::shared_ptr<GShader> shader)
	{
		RenderablePipelineState->SetShader(shader);
	}

	template<typename T>
	inline void GRenderableBase<T>::SetRenderableInputLayout(std::shared_ptr<GInputLayout> inputLayout)
	{
		RenderablePipelineState->SetInputLayout(inputLayout);
	}

	template<typename T>
	inline void GRenderableBase<T>::SetRenderableTopology(std::shared_ptr<GTopology> topology)
	{
		RenderablePipelineState->SetTopology(topology);
	}

	template<typename T>
	inline void GRenderableBase<T>::EndUpInitializing()
	{
		if (!RenderablePipelineState->GetInitialized())
		{
			RenderablePipelineState->InitializePipelineState();
		}
	}

	template<typename T>
	inline const bool GRenderableBase<T>::GetStaticInitialized() const noexcept
	{
		return this->StaticApplicableList.empty() != true;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<GApplicable>> GRenderableBase<T>::GetStaticApplicableList() noexcept
	{
		return StaticApplicableList;
	}

	template<typename T>
	inline std::shared_ptr<GPipelineState> GRenderableBase<T>::GetRenderablePipelineState() noexcept
	{
		return RenderablePipelineState;
	}
}

#endif
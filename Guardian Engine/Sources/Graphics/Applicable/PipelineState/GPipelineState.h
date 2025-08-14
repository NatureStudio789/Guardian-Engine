#ifndef _GE_GPIPELINESTATE_H_
#define _GE_GPIPELINESTATE_H_
#include "GBlendState/GBlendState.h"

namespace GE
{
	class GUARDIAN_API GPipelineState : public GApplicable
	{
	public:
		GPipelineState();
		GPipelineState(const std::string& name);
		GPipelineState(const GPipelineState& other);
		~GPipelineState() override;

		void SetShader(std::shared_ptr<GShader> shader);
		void SetInputLayout(std::shared_ptr<GInputLayout> inputLayout);
		void SetTopology(std::shared_ptr<GTopology> topology);
		void SetRasterizerState(std::shared_ptr<GRasterizerState> rasterizerState);
		void SetBlendState(std::shared_ptr<GBlendState> blendState);
		void InitializePipelineState();

		void Apply() override;

		const std::string& GetPipelineStateName() const noexcept;
		WRL::ComPtr<ID3D12PipelineState> GetPipelineStateObject();
		std::shared_ptr<GRootSignature> GetPipelineRootSignature();
		const bool& GetInitialized() const noexcept;

	private:
		std::string PipelineStateName;
		WRL::ComPtr<ID3D12PipelineState> PipelineStateObject;
		
		std::shared_ptr<GRootSignature> PipelineRootSignature;
		std::map<GShader::Category, std::shared_ptr<GShader>> PipelineShaderList;
		std::shared_ptr<GInputLayout> PipelineInputLayout;
		std::shared_ptr<GTopology> PipelineTopology;

		std::shared_ptr<GRasterizerState> PipelineRasterizerState;
		std::shared_ptr<GBlendState> PipelineBlendState;

		bool IsInitialized;
	};
}

#endif
#ifndef _GE_GPIPELINESTATE_H_
#define _GE_GPIPELINESTATE_H_
#include "Shader/GShader.h"

namespace GE
{
	class GUARDIAN_API GPipelineState : public GApplicable
	{
	public:
		GPipelineState();
		GPipelineState(const GPipelineState& other);
		~GPipelineState() override;

		void SetShader(std::shared_ptr<GShader> shader);
		void InitializePipelineState();

		void Apply() override;

		WRL::ComPtr<ID3D12PipelineState> GetPipelineStateObject();
		const bool& GetInitialized() const noexcept;

	private:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
		WRL::ComPtr<ID3D12PipelineState> PipelineStateObject;
		bool IsInitialized;
	};
}

#endif
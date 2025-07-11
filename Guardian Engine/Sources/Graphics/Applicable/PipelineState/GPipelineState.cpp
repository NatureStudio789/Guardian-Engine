#include "GPipelineState.h"

namespace GE
{
    GPipelineState::GPipelineState()
    {
        GUARDIAN_CLEAR_MEMORY(this->PipelineStateDesc);
        this->PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        this->PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        this->PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        this->PipelineStateDesc.SampleMask = UINT_MAX;
        this->PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        this->PipelineStateDesc.NumRenderTargets = 1;
        this->PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        this->PipelineStateDesc.SampleDesc.Count = 1;
        this->PipelineStateDesc.SampleDesc.Quality = 0;
        this->PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        this->IsInitialized = false;
    }

    GPipelineState::GPipelineState(const GPipelineState& other)
    {
        this->PipelineStateDesc = other.PipelineStateDesc;
        this->PipelineStateObject = other.PipelineStateObject;
        this->IsInitialized = other.IsInitialized;
    }

    GPipelineState::~GPipelineState()
    {
        GUARDIAN_CLEAR_MEMORY(this->PipelineStateDesc);
        this->IsInitialized = false;
    }

    void GPipelineState::SetShader(std::shared_ptr<GShader> shader)
    {
        if (this->IsInitialized)
        {
            throw GUARDIAN_ERROR_EXCEPTION("This pipeline state has been initialized!");
        }

        switch (shader->ShaderCategory)
        {
            case GShader::GE_VERTEX_SHADER:
            {
                this->PipelineStateDesc.VS =
                {
                    .pShaderBytecode = shader->ShaderByteCode->GetBufferPointer(),
                    .BytecodeLength = shader->ShaderByteCode->GetBufferSize()
                };

                break;
            }

            case GShader::GE_PIXEL_SHADER:
            {
                this->PipelineStateDesc.PS =
                {
                    .pShaderBytecode = shader->ShaderByteCode->GetBufferPointer(),
                    .BytecodeLength = shader->ShaderByteCode->GetBufferSize()
                };

                break;
            }

            case GShader::GE_DOMAIN_SHADER:
            {
                this->PipelineStateDesc.DS =
                {
                    .pShaderBytecode = shader->ShaderByteCode->GetBufferPointer(),
                    .BytecodeLength = shader->ShaderByteCode->GetBufferSize()
                };

                break;
            }

            case GShader::GE_HULL_SHADER:
            {
                this->PipelineStateDesc.HS =
                {
                    .pShaderBytecode = shader->ShaderByteCode->GetBufferPointer(),
                    .BytecodeLength = shader->ShaderByteCode->GetBufferSize()
                };

                break;
            }

            case GShader::GE_GEOMETRY_SHADER:
            {
                this->PipelineStateDesc.GS =
                {
                    .pShaderBytecode = shader->ShaderByteCode->GetBufferPointer(),
                    .BytecodeLength = shader->ShaderByteCode->GetBufferSize()
                };

                break;
            }

            default:
            {
                throw GUARDIAN_ERROR_EXCEPTION("Unsupported shader category for pipeline state!");
                break;
            }
        }
    }

    void GPipelineState::InitializePipelineState()
    {
        this->PipelineStateDesc.pRootSignature = GGraphicsContextRegistry::GetCurrentGraphicsContext()->
            GetGraphicsRootSignature()->GetRootSignatureObject().Get();
    }

    void GPipelineState::Apply()
    {
        if (!this->IsInitialized)
        {
            throw GUARDIAN_ERROR_EXCEPTION("This pipeline state has NOT been initialized!");
        }

        GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
            SetPipelineState(this->PipelineStateObject.Get());
    }

    WRL::ComPtr<ID3D12PipelineState> GPipelineState::GetPipelineStateObject()
    {
        return this->PipelineStateObject;
    }

    const bool& GPipelineState::GetInitialized() const noexcept
    {
        return this->IsInitialized;
    }
}

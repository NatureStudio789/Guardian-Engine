#include "GPipelineState.h"

namespace GE
{
    GPipelineState::GPipelineState()
    {
        this->PipelineStateName = "";
        this->PipelineRootSignature = std::make_shared<GRootSignature>();
        this->PipelineInputLayout = null;
        this->PipelineShaderList.clear();
        this->PipelineTopology = null;

        this->PipelineRasterizerState = null;
        this->PipelineBlendState = null;

        this->IsInitialized = false;
    }

    GPipelineState::GPipelineState(const std::string& name) : GPipelineState()
    {
        this->PipelineStateName = name;
    }

    GPipelineState::GPipelineState(const GPipelineState& other)
    {
        this->PipelineStateName = other.PipelineStateName;
        this->PipelineStateObject = other.PipelineStateObject;
        
        this->PipelineRootSignature = other.PipelineRootSignature;
        this->PipelineInputLayout = other.PipelineInputLayout;
        this->PipelineShaderList = other.PipelineShaderList;
        this->PipelineTopology = other.PipelineTopology;

        this->PipelineRasterizerState = null;
        this->PipelineBlendState = null;

        this->IsInitialized = other.IsInitialized;
    }

    GPipelineState::~GPipelineState()
    {
        this->PipelineStateName = "";
        this->PipelineRootSignature.reset();
        this->PipelineRootSignature = null;

        this->PipelineInputLayout.reset();
        this->PipelineInputLayout = null;
        for (auto& shader : this->PipelineShaderList)
        {
            shader.second.reset();
            shader.second = null;
        }
        this->PipelineShaderList.clear();
        this->PipelineTopology = null;

        this->PipelineRasterizerState = null;
        this->PipelineBlendState = null;

        this->IsInitialized = false;
    }

    void GPipelineState::SetShader(std::shared_ptr<GShader> shader)
    {
        if (this->IsInitialized)
        {
            throw GUARDIAN_ERROR_EXCEPTION("This pipeline state has been initialized!");
        }

        if (this->PipelineShaderList.count(shader->GetShaderCategory()))
        {
            throw GUARDIAN_ERROR_EXCEPTION("This type of shader has already been in pipeline!");
        }

        this->PipelineShaderList[shader->GetShaderCategory()] = shader;
    }

    void GPipelineState::SetInputLayout(std::shared_ptr<GInputLayout> inputLayout)
    {
        if (this->IsInitialized)
        {
            throw GUARDIAN_ERROR_EXCEPTION("This pipeline state has been initialized!");
        }

        this->PipelineInputLayout = inputLayout;
    }

    void GPipelineState::SetTopology(std::shared_ptr<GTopology> topology)
    {
        this->PipelineTopology = topology;
    }

    void GPipelineState::SetRasterizerState(std::shared_ptr<GRasterizerState> rasterizerState)
    {
        this->PipelineRasterizerState = rasterizerState;
    }

    void GPipelineState::SetBlendState(std::shared_ptr<GBlendState> blendState)
    {
        this->PipelineBlendState = blendState;
    }

    void GPipelineState::InitializePipelineState()
    {
        GUARDIAN_SETUP_AUTO_THROW();

        this->PipelineRootSignature->InitializeRootSignature(
            GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice());

        D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
        GUARDIAN_CLEAR_MEMORY(PipelineStateDesc);

        PipelineStateDesc.pRootSignature = this->PipelineRootSignature->GetRootSignatureObject().Get();
        PipelineStateDesc.InputLayout = { this->PipelineInputLayout->GetInputElementList(), this->PipelineInputLayout->GetInputElementCount() };

        PipelineStateDesc.VS =
        {
            this->PipelineShaderList[GShader::GE_VERTEX_SHADER]->GetShaderByteCode()->GetBufferPointer(),
            this->PipelineShaderList[GShader::GE_VERTEX_SHADER]->GetShaderByteCode()->GetBufferSize()
        };
        PipelineStateDesc.PS =
        {
            this->PipelineShaderList[GShader::GE_PIXEL_SHADER]->GetShaderByteCode()->GetBufferPointer(),
            this->PipelineShaderList[GShader::GE_PIXEL_SHADER]->GetShaderByteCode()->GetBufferSize()
        };

        auto RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        if (this->PipelineRasterizerState)
        {
            RS.FillMode = (D3D12_FILL_MODE)this->PipelineRasterizerState->GetFillMode();
            RS.CullMode = (D3D12_CULL_MODE)this->PipelineRasterizerState->GetCullMode();
        }
        PipelineStateDesc.RasterizerState = RS;

        auto BS = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        if (this->PipelineBlendState)
        {
            D3D12_RENDER_TARGET_BLEND_DESC RenderTargetBlendDesc;
            GUARDIAN_CLEAR_MEMORY(RenderTargetBlendDesc);
            RenderTargetBlendDesc.BlendEnable = true;
            RenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
            RenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
            RenderTargetBlendDesc.DestBlend = D3D12_BLEND_DEST_COLOR;
            RenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
            RenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
            RenderTargetBlendDesc.LogicOpEnable = false;
            RenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
            RenderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_COLOR;
            RenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;

            BS.RenderTarget[0] = RenderTargetBlendDesc;
        }
        PipelineStateDesc.BlendState = BS;

        PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        PipelineStateDesc.SampleMask = UINT_MAX;

        D3D12_PRIMITIVE_TOPOLOGY_TYPE Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
        if (this->PipelineTopology->GetTopologyType() == GTopology::GE_TOPOLOGY_TYPE_POINTLIST)
        {
            Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        }
        else if (this->PipelineTopology->GetTopologyType() == GTopology::GE_TOPOLOGY_TYPE_LINELIST ||
            this->PipelineTopology->GetTopologyType() == GTopology::GE_TOPOLOGY_TYPE_LINESTRIP)
        {
            Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        }
        else if (this->PipelineTopology->GetTopologyType() == GTopology::GE_TOPOLOGY_TYPE_TRIANGLELIST ||
            this->PipelineTopology->GetTopologyType() == GTopology::GE_TOPOLOGY_TYPE_TRIANGLESTRIP)
        {
            Type = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        }
        PipelineStateDesc.PrimitiveTopologyType = Type;

        PipelineStateDesc.NumRenderTargets = 1;
        PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        PipelineStateDesc.SampleDesc.Count = 1;
        PipelineStateDesc.SampleDesc.Quality = 0;
        PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

        GUARDIAN_AUTO_THROW(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsDevice()->GetDeviceObject()->
            CreateGraphicsPipelineState(&PipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)this->PipelineStateObject.GetAddressOf()));

        this->IsInitialized = true;
    }

    void GPipelineState::Apply()
    {
        if (!this->IsInitialized)
        {
            throw GUARDIAN_ERROR_EXCEPTION("This pipeline state has NOT been initialized!");
        }

        this->PipelineRootSignature->ApplyRootSignature(GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList());

        GGraphicsContextRegistry::GetCurrentGraphicsContext()->GetGraphicsCommandList()->GetCommandListObject()->
            SetPipelineState(this->PipelineStateObject.Get());
    }

    const std::string& GPipelineState::GetPipelineStateName() const noexcept
    {
        return this->PipelineStateName;
    }

    WRL::ComPtr<ID3D12PipelineState> GPipelineState::GetPipelineStateObject()
    {
        return this->PipelineStateObject;
    }

    std::shared_ptr<GRootSignature> GPipelineState::GetPipelineRootSignature()
    {
        return this->PipelineRootSignature;
    }

    const bool& GPipelineState::GetInitialized() const noexcept
    {
        return this->IsInitialized;
    }
}

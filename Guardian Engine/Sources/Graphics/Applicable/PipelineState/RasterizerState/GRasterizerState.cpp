#include "GRasterizerState.h"

namespace GE
{
    GRasterizerState::GRasterizerState()
    {
        this->Fill = GE_FILL_SOLID;
        this->Cull = GE_CULL_BACK;
    }

    GRasterizerState::GRasterizerState(FillMode fill, CullMode cull)
    {
        this->InitializeRasterizerState(fill, cull);
    }

    GRasterizerState::GRasterizerState(const GRasterizerState& other)
    {
        this->Fill = other.Fill;
        this->Cull = other.Cull;
    }

    GRasterizerState::~GRasterizerState()
    {

    }

    void GRasterizerState::InitializeRasterizerState(FillMode fill, CullMode cull)
    {
        this->Fill = fill;
        this->Cull = cull;
    }

    const GRasterizerState::FillMode& GRasterizerState::GetFillMode()
    {
        return this->Fill;
    }

    const GRasterizerState::CullMode& GRasterizerState::GetCullMode()
    {
        return this->Cull;
    }
}

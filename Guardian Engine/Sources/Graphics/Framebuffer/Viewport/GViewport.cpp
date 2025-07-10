#include "GViewport.h"

namespace GE
{
    GViewport::GViewport()
    {
        GUARDIAN_CLEAR_MEMORY(this->ViewportObject);
        GUARDIAN_CLEAR_MEMORY(this->ViewportAttribute);
    }

    GViewport::GViewport(const Attribute& attribute)
    {
        this->SetViewportAttribute(attribute);
    }

    GViewport::GViewport(const GViewport& other)
    {
        this->ViewportObject = other.ViewportObject;
        this->ViewportAttribute = other.ViewportAttribute;
    }

    GViewport::~GViewport()
    {
        GUARDIAN_CLEAR_MEMORY(this->ViewportObject);
        GUARDIAN_CLEAR_MEMORY(this->ViewportAttribute);
    }

    void GViewport::SetViewportAttribute(const Attribute& attribute)
    {
        this->ViewportAttribute = attribute;

        this->ViewportObject.TopLeftX = this->ViewportAttribute.PositionX;
        this->ViewportObject.TopLeftY = this->ViewportAttribute.PositionY;
        this->ViewportObject.Width = this->ViewportAttribute.Width;
        this->ViewportObject.Height = this->ViewportAttribute.Height;
        this->ViewportObject.MaxDepth = this->ViewportAttribute.MaxDepth;
        this->ViewportObject.MinDepth = this->ViewportAttribute.MinDepth;
    }

    void GViewport::ApplyViewport(std::shared_ptr<GCommandList> commandList)
    {
        commandList->GetCommandListObject()->RSSetViewports(1, &this->ViewportObject);
    }

    const D3D12_VIEWPORT& GViewport::GetViewportObject() const noexcept
    {
        return this->ViewportObject;
    }

    const GViewport::Attribute& GViewport::GetViewportAttribute() const noexcept
    {
        return this->ViewportAttribute;
    }
}

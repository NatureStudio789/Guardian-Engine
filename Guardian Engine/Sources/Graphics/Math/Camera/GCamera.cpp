#include "GCamera.h"

namespace GE
{
    GCamera::GCamera()
    {
        this->Position = {};
        this->Rotation = {};
        this->Projection = {};
        this->IsFreeLook = true;
    }

    GCamera::GCamera(const GVector3& position, const GVector3& direction, 
        const GPerspectiveProjection& projection, bool isFreeLook)
    {
        this->Position = position;
        this->Rotation = direction;
        this->Projection = projection;
        this->IsFreeLook = isFreeLook;
    }

    GCamera::GCamera(const GCamera& other)
    {
        this->Position = other.Position;
        this->Rotation = other.Rotation;
        this->Projection = other.Projection;
        this->IsFreeLook = other.IsFreeLook;
    }

    GCamera::~GCamera()
    {
        this->Position = {};
        this->Rotation = {};
        this->Projection = {};
        this->IsFreeLook = false;
    }

    void GCamera::Translate(const GVector3& translation)
    {
        this->Position += translation;
    }

    void GCamera::Rotate(const GVector3& rotation)
    {
        this->Rotation += rotation;
    }

    const GMatrix GCamera::GetViewMatrix() const noexcept
    {
        return GMatrix::LookAtMatrix(this->Position, GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, 1.0f)) + this->Position, 
            GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 1.0f, 0.0f)));
    }

    const GVector3 GCamera::GetForwardVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, 1.0f));
    }

    const GVector3 GCamera::GetBackwardVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, -1.0f));
    }

    const GVector3 GCamera::GetLeftVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(-1.0f, 0.0f, 0.0f));
    }

    const GVector3 GCamera::GetRightVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(1.0f, 0.0f, 0.0f));
    }

    const GVector3 GCamera::GetUpVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 1.0f, 0.0f));
    }

    const GVector3 GCamera::GetDownVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, -1.0f, 0.0f));
    }

    const GMatrix GCamera::GetRotationMatrix() const noexcept
    {
        return GMatrix::RotationMatrix(this->Rotation);
    }
}

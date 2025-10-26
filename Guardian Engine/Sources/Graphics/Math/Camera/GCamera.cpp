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

    void GCamera::ResizeFrustum(float newWidth, float newHeight)
    {
        this->Projection.Aspect = newWidth / newHeight;
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

    GOrthoCamera::GOrthoCamera()
    {
        this->Position = {};
        this->Rotation = {};
        this->Projection = {};
        this->IsFreeLook = true;
    }

    GOrthoCamera::GOrthoCamera(const GVector3& position, const GVector3& direction,
        const GOrthographicsProjection& projection, bool isFreeLook)
    {
        this->Position = position;
        this->Rotation = direction;
        this->Projection = projection;
        this->IsFreeLook = isFreeLook;
    }

    GOrthoCamera::GOrthoCamera(const GOrthoCamera& other)
    {
        this->Position = other.Position;
        this->Rotation = other.Rotation;
        this->Projection = other.Projection;
        this->IsFreeLook = other.IsFreeLook;
    }

    GOrthoCamera::~GOrthoCamera()
    {
        this->Position = {};
        this->Rotation = {};
        this->Projection = {};
        this->IsFreeLook = false;
    }

    void GOrthoCamera::Translate(const GVector3& translation)
    {
        this->Position += translation;
    }

    void GOrthoCamera::Rotate(const GVector3& rotation)
    {
        this->Rotation += rotation;
    }

    void GOrthoCamera::ResizeFrustum(float newWidth, float newHeight)
    {
        this->Projection.ViewWidth = newWidth;
        this->Projection.ViewHeight = newHeight;
    }

    const GMatrix GOrthoCamera::GetViewMatrix() const noexcept
    {
        return GMatrix::LookAtMatrix(this->Position, GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, 1.0f)) + this->Position,
            GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 1.0f, 0.0f)));
    }

    const GVector3 GOrthoCamera::GetForwardVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, 1.0f));
    }

    const GVector3 GOrthoCamera::GetBackwardVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 0.0f, -1.0f));
    }

    const GVector3 GOrthoCamera::GetLeftVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(-1.0f, 0.0f, 0.0f));
    }

    const GVector3 GOrthoCamera::GetRightVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(1.0f, 0.0f, 0.0f));
    }

    const GVector3 GOrthoCamera::GetUpVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, 1.0f, 0.0f));
    }

    const GVector3 GOrthoCamera::GetDownVector() const noexcept
    {
        return GMatrix::MultiplyCoord(this->GetRotationMatrix(), GVector3(0.0f, -1.0f, 0.0f));
    }

    const GMatrix GOrthoCamera::GetRotationMatrix() const noexcept
    {
        return GMatrix::RotationMatrix(this->Rotation);
    }
}

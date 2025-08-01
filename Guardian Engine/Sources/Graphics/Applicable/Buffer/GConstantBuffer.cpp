#include "GConstantBuffer.h"
#include "../../Renderer/Renderable/GRenderable.h"

namespace GE
{
	void GTransformCBuffer::SetParent(const GRenderable& renderable)
	{
		this->Parent = &renderable;
	}

	void GTransformCBuffer::Apply()
	{
		GConstantBuffer<GTransformCBData>::Apply();

		GMatrix WorldMatrix = this->Parent->GetTransform().GetTransformMatrix() * this->Parent->GetExtraMatrix() * this->Parent->GetAccumulatedMatrix();
		WorldMatrix.Transpose();

		this->UpdateBufferData({ WorldMatrix });
	}
}

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

		GMatrix Matrix = this->Parent->GetTransform().GetTransformMatrix();
		Matrix.Transpose();
		this->UpdateBufferData({ Matrix });
	}
}

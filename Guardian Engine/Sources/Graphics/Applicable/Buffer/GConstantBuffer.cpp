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

		GMatrix Matrix = this->Parent->GetTransform().GetTransformMatrix()/* * this->Parent->GetExtraMatrix() * this->Parent->GetAccumulatedMatrix()*/ * 
			XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) * 
			XMMatrixPerspectiveFovLH((60.0f / 360.0f) * XM_2PI, 16.0f / 9.0f, 0.01f, 1000.0f);
		Matrix.Transpose();
		this->UpdateBufferData({ Matrix });
	}
}

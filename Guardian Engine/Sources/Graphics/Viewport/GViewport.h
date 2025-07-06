#ifndef _GE_GVIEWPORT_H_
#define _GE_GVIEWPORT_H_
#include "../RenderTarget/GRenderTarget.h"

namespace GE
{
	class GUARDIAN_API GViewport
	{
	public:
		struct Attribute
		{
			Attribute()
			{
				this->PositionX = this->PositionY = 0.0f;
				this->Width = this->Height = 800.0f;
				this->MinDepth = 0.0f;
				this->MaxDepth = 1.0f;
			}
			Attribute(const Attribute& other)
			{
				this->PositionX = other.PositionX;
				this->PositionY = other.PositionY;
				this->Width = other.Width;
				this->Height = other.Height;
				this->MinDepth = other.MinDepth;
				this->MaxDepth = other.MaxDepth;
			}

			float PositionX;
			float PositionY;
			float Width;
			float Height;
			float MinDepth;
			float MaxDepth;
		};

	public:
		GViewport();
		GViewport(const Attribute& attribute);
		GViewport(const GViewport& other);
		~GViewport();

		void SetViewportAttribute(const Attribute& attribute);

		void ApplyViewport(std::shared_ptr<GCommandList> commandList);

		const D3D12_VIEWPORT& GetViewportObject() const noexcept;
		const Attribute& GetViewportAttribute() const noexcept;

		static std::shared_ptr<GViewport> CreateNewViewport(const Attribute& attribute)
		{
			return std::make_shared<GViewport>(attribute);
		}

	private:
		D3D12_VIEWPORT ViewportObject;
		Attribute ViewportAttribute;
	};
}

#endif
#ifndef _GE_GUARDIANSURFACE_H_
#define _GE_GUARDIANSURFACE_H_
#include "../Viewport/GuardianViewport.h"

namespace GE
{
	class GuardianColor
	{
	public:
		unsigned int ColorValue;
	public:
		constexpr GuardianColor() : ColorValue()
		{
		}
		constexpr GuardianColor(const GuardianColor& col)
			:
			ColorValue(col.ColorValue)
		{
		}
		constexpr GuardianColor(unsigned int dw)
			:
			ColorValue(dw)
		{
		}
		constexpr GuardianColor(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
			:
			ColorValue((x << 24u) | (r << 16u) | (g << 8u) | b)
		{
		}
		constexpr GuardianColor(unsigned char r, unsigned char g, unsigned char b)
			:
			ColorValue((255u << 24u) | (r << 16u) | (g << 8u) | b)
		{
		}
		constexpr GuardianColor(GuardianColor col, unsigned char x)
			:
			GuardianColor((x << 24u) | col.ColorValue)
		{
		}

		GuardianColor& operator =(GuardianColor color)
		{
			ColorValue = color.ColorValue;
			return *this;
		}

		operator UINT()
		{
			return this->ColorValue;
		}

		constexpr unsigned char GetX() const
		{
			return ColorValue >> 24u;
		}
		constexpr unsigned char GetA() const
		{
			return GetX();
		}
		constexpr unsigned char GetR() const
		{
			return (ColorValue >> 16u) & 0xFFu;
		}
		constexpr unsigned char GetG() const
		{
			return (ColorValue >> 8u) & 0xFFu;
		}
		constexpr unsigned char GetB() const
		{
			return ColorValue & 0xFFu;
		}
		void SetX(unsigned char x)
		{
			ColorValue = (ColorValue & 0xFFFFFFu) | (x << 24u);
		}
		void SetA(unsigned char a)
		{
			SetX(a);
		}
		void SetR(unsigned char r)
		{
			ColorValue = (ColorValue & 0xFF00FFFFu) | (r << 16u);
		}
		void SetG(unsigned char g)
		{
			ColorValue = (ColorValue & 0xFFFF00FFu) | (g << 8u);
		}
		void SetB(unsigned char b)
		{
			ColorValue = (ColorValue & 0xFFFFFF00u) | b;
		}
	};

	class GUARDIAN_API GuardianSurface
	{
	public:
		GuardianSurface();
		GuardianSurface(unsigned int width, unsigned int height);
		GuardianSurface(GString imageFileName);
		GuardianSurface(GuardianSurface&&) = default;
		GuardianSurface(const GuardianSurface&) = delete;
		~GuardianSurface() = default;

		void InitializeSurface(unsigned int width, unsigned int height);
		void InitializeSurface(GString imageFileName);

		void ClearSurface(GuardianColor fillValue);
		void ReplacePixel(unsigned int x, unsigned int y, GuardianColor replaceColor);
		void SaveSurface(GString savePath);

		const GuardianColor& GetPixel(unsigned int x, unsigned int y) const noexcept;
		const int GetSurfaceWidth() const noexcept;
		const int GetSurfaceHeight() const noexcept;
		const int GetBytePitch() const noexcept;
		GuardianColor* GetBufferData() const noexcept;

		static std::shared_ptr<GuardianSurface> CreateNewSurface(unsigned int width, unsigned int height);
		static std::shared_ptr<GuardianSurface> CreateNewSurface(GString imageFileName);

	private:
		DirectX::ScratchImage SurfaceImage;
	};
}

#endif
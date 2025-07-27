#ifndef _GE_GSURFACE_H_
#define _GE_GSURFACE_H_
#include "../GGraphicsContext.h"

namespace GE
{
	class GUARDIAN_API GColor
	{
	public:
		GColor() : ColorValue()
		{

		}
		GColor(unsigned int color)
		{
			this->ColorValue = color;
		}
		GColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			this->ColorValue = ((a << 24) | (r << 16) | (g << 8) | b);
		}
		GColor(unsigned char r, unsigned char g, unsigned char b) : GColor(r, g, b, 255)
		{

		}
		GColor(GColor color, unsigned char a) : GColor((a << 24) | color.ColorValue)
		{
			
		}
		GColor(const GColor& other)
		{
			this->ColorValue = other.ColorValue;
		}

		void SetA(unsigned char a)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (a << 24);
		}
		void SetR(unsigned char r)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (r << 16);
		}
		void SetG(unsigned char g)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | (g << 8);
		}
		void SetB(unsigned char b)
		{
			this->ColorValue = (this->ColorValue & 0xFFFFFF) | b;
		}

		GColor& operator=(GColor other)
		{
			this->ColorValue = other.ColorValue;
			return *this;
		}

		operator unsigned int()
		{
			return this->ColorValue;
		}

		const unsigned char GetA() const noexcept
		{
			return this->ColorValue >> 24;
		}
		const unsigned char GetR() const noexcept
		{
			return (this->ColorValue >> 16) & 0xFF;
		}
		const unsigned char GetG() const noexcept
		{
			return (this->ColorValue >> 8) & 0xFF;
		}
		const unsigned char GetB() const noexcept
		{
			return this->ColorValue & 0xFF;
		}

	public:
		unsigned int ColorValue;
	};

	class GUARDIAN_API GSurface
	{
	public:
		GSurface();
		GSurface(unsigned int width, unsigned int height);
		GSurface(const std::string& filePath);
		GSurface(GSurface&&) = default;
		GSurface(const GSurface&) = delete;
		~GSurface() = default;

		void InitializeSurface(unsigned int width, unsigned int height);
		void InitializeSurface(const std::string& filePath);

		void ClearSurface(GColor fillValue);
		void ReplacePixel(unsigned int x, unsigned int y, GColor replaceColor);
		void SaveSurface(const std::string& savePath);

		const GColor& GetPixel(unsigned int x, unsigned int y) const noexcept;
		const int GetSurfaceWidth() const noexcept;
		const int GetSurfaceHeight() const noexcept;
		const int GetBytePitch() const noexcept;
		GColor* GetBufferData() const noexcept;

		static std::shared_ptr<GSurface> CreateNewSurface(unsigned int width, unsigned height)
		{
			return std::make_shared<GSurface>(width, height);
		}
		static std::shared_ptr<GSurface> CreateNewSurface(const std::string& filePath)
		{
			return std::make_shared<GSurface>(filePath);
		}

	private:
		ScratchImage SurfaceImage;
	};
}

#endif
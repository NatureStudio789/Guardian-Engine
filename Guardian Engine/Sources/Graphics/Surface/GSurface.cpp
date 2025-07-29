#include "GSurface.h"

namespace GE
{
	GSurface::GSurface()
	{
		this->SurfaceImage = ScratchImage();
	}

	GSurface::GSurface(unsigned int width, unsigned int height)
	{
		this->InitializeSurface(width, height);
	}

	GSurface::GSurface(const std::string& filePath)
	{
		this->InitializeSurface(filePath);
	}

	void GSurface::InitializeSurface(unsigned int width, unsigned int height)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(this->SurfaceImage.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1));
	}

	void GSurface::InitializeSurface(const std::string& filePath)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		GUARDIAN_AUTO_THROW(CoInitializeEx(null, COINIT_MULTITHREADED));

		GUARDIAN_AUTO_THROW(LoadFromWICFile(GUtil::StringToWideString(filePath).c_str(), 
			WIC_FLAGS_IGNORE_SRGB, null, this->SurfaceImage));

		if (this->SurfaceImage.GetMetadata().format != DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			ScratchImage ConvertedImage;
			GUARDIAN_AUTO_THROW(Convert(*this->SurfaceImage.GetImage(0, 0, 0), DXGI_FORMAT_R8G8B8A8_UNORM,
				TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT, ConvertedImage));

			this->SurfaceImage = std::move(ConvertedImage);
		}
	}

	void GSurface::ClearSurface(GColor fillValue)
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);

		for (int y = 0; y < this->GetSurfaceHeight(); y++)
		{
			auto RowStart = (GColor*)(ImageData.pixels + ImageData.rowPitch * y);
			std::fill(RowStart, RowStart + this->GetSurfaceWidth(), fillValue);
		}
	}

	void GSurface::ReplacePixel(unsigned int x, unsigned int y, GColor replaceColor)
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);
		reinterpret_cast<GColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x] = replaceColor;
	}

	void GSurface::SaveSurface(const std::string& savePath)
	{
		GUARDIAN_SETUP_AUTO_THROW();

		const auto GetCodecId = [](const std::string& savePath)
		{
			const std::filesystem::path filePath = savePath;
			if (filePath.extension().string() == ".png")
			{
				return WIC_CODEC_PNG;
			}
			else if (filePath.extension().string() == ".jpg")
			{
				return WIC_CODEC_JPEG;
			}
			else if (filePath.extension().string() == ".bmp")
			{
				return WIC_CODEC_BMP;
			}
			else
			{
				return (WICCodecs)0;
			}
		};

		GUARDIAN_AUTO_THROW(DirectX::SaveToWICFile(*this->SurfaceImage.GetImage(0, 0, 0), WIC_FLAGS_NONE,
			GetWICCodec(GetCodecId(savePath)), GUtil::StringToWideString(savePath).c_str()));
	}

	const GColor& GSurface::GetPixel(unsigned int x, unsigned int y) const noexcept
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);
		return reinterpret_cast<GColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x];
	}

	const int GSurface::GetSurfaceWidth() const noexcept
	{
		return (int)this->SurfaceImage.GetMetadata().width;
	}

	const int GSurface::GetSurfaceHeight() const noexcept
	{
		return (int)this->SurfaceImage.GetMetadata().height;
	}

	const int GSurface::GetBytePitch() const noexcept
	{
		return (int)this->SurfaceImage.GetImage(0, 0, 0)->rowPitch;
	}

	const int GSurface::GetSlicePitch() const noexcept
	{
		return (int)this->SurfaceImage.GetImage(0, 0, 0)->slicePitch;
	}

	GColor* GSurface::GetBufferData() const noexcept
	{
		return (GColor*)this->SurfaceImage.GetPixels();
	}
}

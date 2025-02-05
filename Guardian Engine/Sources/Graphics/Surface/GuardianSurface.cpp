#include "GuardianSurface.h"

namespace guardian
{
	GuardianSurface::GuardianSurface()
	{
		this->SurfaceImage = ScratchImage();
	}

	GuardianSurface::GuardianSurface(unsigned int width, unsigned int height)
	{
		this->InitializeSurface(width, height);
	}

	GuardianSurface::GuardianSurface(GString imageFileName)
	{
		this->InitializeSurface(imageFileName);
	}

	void GuardianSurface::InitializeSurface(unsigned int width, unsigned int height)
	{
		HRESULT hr = this->SurfaceImage.Initialize2D(DXGI_FORMAT_R8G8B8A8_UNORM, width, height, 1, 1);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	void GuardianSurface::InitializeSurface(GString imageFileName)
	{
		HRESULT hr = CoInitialize(null);
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}

		GWideString WidePath = GuardianConverter::StringToWideString(imageFileName);
		hr = DirectX::LoadFromWICFile(WidePath.c_str(), WIC_FLAGS_IGNORE_SRGB, null, this->SurfaceImage);
		if (GFailed(hr))
		{
			this->InitializeSurface(1, 1);
			this->ClearSurface(GuardianColor(51, 76, 76));
		}

		if (this->SurfaceImage.GetImage(0, 0, 0)->format != DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			ScratchImage ConvertedImage;
			hr = DirectX::Convert(*this->SurfaceImage.GetImage(0, 0, 0), DXGI_FORMAT_R8G8B8A8_UNORM,
				TEX_FILTER_DEFAULT, TEX_THRESHOLD_DEFAULT, ConvertedImage);
			if (GFailed(hr))
			{
				throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
			}

			this->SurfaceImage = std::move(ConvertedImage);
		}
	}

	void GuardianSurface::ClearSurface(GuardianColor fillValue)
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);
		for (int y = 0; y < this->GetSurfaceHeight(); y++)
		{
			auto RowStart = reinterpret_cast<GuardianColor*>(ImageData.pixels + ImageData.rowPitch * y);
			std::fill(RowStart, RowStart + this->GetSurfaceWidth(), fillValue);
		}
	}

	void GuardianSurface::ReplacePixel(unsigned int x, unsigned int y, GuardianColor replaceColor)
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);
		reinterpret_cast<GuardianColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x] = replaceColor;
	}

	void GuardianSurface::SaveSurface(GString savePath)
	{
		const auto GetCodecId = [](GString savePath)
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

		HRESULT hr = DirectX::SaveToWICFile(*this->SurfaceImage.GetImage(0, 0, 0), WIC_FLAGS_NONE,
			GetWICCodec(GetCodecId(savePath)), GuardianConverter::StringToWideString(savePath).c_str());
		if (GFailed(hr))
		{
			throw GUARDIAN_GRAPHICS_EXCEPTION(hr);
		}
	}

	const GuardianColor& GuardianSurface::GetPixel(unsigned int x, unsigned int y) const noexcept
	{
		auto& ImageData = *this->SurfaceImage.GetImage(0, 0, 0);
		return reinterpret_cast<GuardianColor*>(&ImageData.pixels[y * ImageData.rowPitch])[x];
	}

	const int GuardianSurface::GetSurfaceWidth() const noexcept
	{
		return (int)this->SurfaceImage.GetMetadata().width;
	}

	const int GuardianSurface::GetSurfaceHeight() const noexcept
	{
		return (int)this->SurfaceImage.GetMetadata().height;
	}

	const int GuardianSurface::GetBytePitch() const noexcept
	{
		return (int)this->SurfaceImage.GetImage(0, 0, 0)->rowPitch;
	}

	GuardianColor* GuardianSurface::GetBufferData() const noexcept
	{
		return reinterpret_cast<GuardianColor*>(this->SurfaceImage.GetPixels());
	}

	std::shared_ptr<GuardianSurface> GuardianSurface::CreateNewSurface(unsigned int width, unsigned int height)
	{
		return std::make_shared<GuardianSurface>(width, height);
	}

	std::shared_ptr<GuardianSurface> GuardianSurface::CreateNewSurface(GString imageFileName)
	{
		return std::make_shared<GuardianSurface>(imageFileName);
	}
}

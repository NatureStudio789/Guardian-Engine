#ifndef _GE_GUARDIANTEXTURE_H_
#define _GE_GUARDIANTEXTURE_H_
#include "../Buffer/GuardianConstantBuffer.h"

namespace GE
{
	class GUARDIAN_API GuardianTexture : public GuardianApplicable
	{
	public:
		GuardianTexture();
		GuardianTexture(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianSurface& surface, int index = 0);
		GuardianTexture(const GuardianTexture& other);
		~GuardianTexture() override;

		void SetTextureAppliedSlot(int index);
		void InitializeTexture(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianSurface& surface, int index = 0);

		void Apply(std::shared_ptr<GuardianGraphics> graphics) override;

		const GuardianUUID& GetTextureId() const noexcept;
		WRL::ComPtr<ID3D11Texture2D> GetTextureObject();
		WRL::ComPtr<ID3D11ShaderResourceView> GetTextureResource();

		bool operator==(const GuardianTexture& other) const;

		static std::shared_ptr<GuardianTexture> CreateNewTexture(std::shared_ptr<GuardianGraphics> graphics,
			const GuardianSurface& surface, int index = 0);

	private:
		GuardianUUID TextureId;

		WRL::ComPtr<ID3D11Texture2D> TextureObject;
		WRL::ComPtr<ID3D11ShaderResourceView> TextureResource;
		UINT TextureAppliedSlot;
	};
}

#endif
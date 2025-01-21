#ifndef _GE_GUARDIANDEPTHSTENCIL_H_
#define _GE_GUARDIANDEPTHSTENCIL_H_
#include "../Camera/GuardianCamera.h"

namespace guardian
{
	class GUARDIAN_API GuardianGraphics;

	class GUARDIAN_API GuardianDepthStencil
	{
	public:
		GuardianDepthStencil();
		GuardianDepthStencil(std::shared_ptr<GuardianGraphics> graphics, int width, int height);
		GuardianDepthStencil(const GuardianDepthStencil& other);
		~GuardianDepthStencil();

		void InitializeDepthStencil(std::shared_ptr<GuardianGraphics> graphics, int width, int height);

		WRL::ComPtr<ID3D11DepthStencilState> GetDepthStencilState() noexcept;
		WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() noexcept;
		WRL::ComPtr<ID3D11ShaderResourceView> GetDepthStencilShaderResource() noexcept;

		static std::shared_ptr<GuardianDepthStencil> CreateNewDepthStencil(
			std::shared_ptr<GuardianGraphics> graphics, int width, int height);

	private:
		WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
		WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
		WRL::ComPtr<ID3D11ShaderResourceView> DepthStencilShaderResource;
	};
}

#endif
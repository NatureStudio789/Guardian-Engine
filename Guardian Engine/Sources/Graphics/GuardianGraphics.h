#ifndef _GE_GUARDIANGRAPHICS_H_
#define _GE_GUARDIANGRAPHICS_H_
#include "Framebuffer/GuardianFramebuffer.h"

namespace GE
{
	class GUARDIAN_API GuardianGraphics
	{
	public:
		GuardianGraphics();
		GuardianGraphics(const GuardianGraphics& ) = default;
		GuardianGraphics(GWindowHandle renderWindow,
			int windowWidth, int windowHeight, bool isFullscreenWindow);
		~GuardianGraphics() = default;

		void InitializeGraphics(GWindowHandle renderWindow, 
			int windowWidth, int windowHeight, bool isFullscreenWindow);

		void EndUpRendering(int syncInterval = 1);

		void UpdateGUIGraphicsResolution(int newWidth, int newHeight);

		WRL::ComPtr<ID3D11Device> GetGraphicsDevice() noexcept;
		WRL::ComPtr<ID3D11DeviceContext> GetGraphicsDeviceContext() noexcept;
		WRL::ComPtr<IDXGISwapChain> GetGraphicsSwapChain() noexcept;
		std::shared_ptr<GuardianFramebuffer> GetGraphicsMainFramebuffer() noexcept;

		static std::shared_ptr<GuardianGraphics> CreateNewGraphics(GWindowHandle renderWindow,
			int windowWidth, int windowHeight, bool isFullscreenWindow);

	private:
		WRL::ComPtr<ID3D11Device> GraphicsDevice;
		WRL::ComPtr<ID3D11DeviceContext> GraphicsDeviceContext;
		WRL::ComPtr<IDXGISwapChain> GraphicsSwapChain;

		std::shared_ptr<GuardianFramebuffer> GraphicsMainFramebuffer;
	};
}

#endif
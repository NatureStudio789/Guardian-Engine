#ifndef _GE_GWINDOW_H_
#define _GE_GWINDOW_H_
#include "../../Graphics/Renderer/GRenderer.h"

namespace GE
{
	class GUARDIAN_API GWindow
	{
	public:
		enum Style
		{
			GE_STYLE_DEFAULTWINDOW,
			GE_STYLE_FULLSCREEN
		};

		enum Theme
		{
			GE_THEME_DARK,
			GE_THEME_LIGHT,
			GE_THEME_BLUE
		};

		struct Attribute
		{
		public:
			Attribute() = default;
			Attribute(const std::string& title, const Style& style, const Theme& theme)
			{
				this->WindowTitle = title;
				this->WindowStyle = style;
				this->WindowTheme = theme;

				switch (this->WindowStyle)
				{
					case GE_STYLE_DEFAULTWINDOW:
					{
						this->EnableWindowTitleBar = true;
						break;
					}

					case GE_STYLE_FULLSCREEN:
					{
						this->EnableWindowTitleBar = false;
						break;
					}
				}
			}
			Attribute(const Attribute& other)
			{
				this->WindowTitle = other.WindowTitle;
				this->WindowStyle = other.WindowStyle;
				this->WindowTheme = other.WindowTheme;
			}

			void SetTitleBarEnable(bool enable)
			{
				if (this->WindowStyle == GE_STYLE_FULLSCREEN)
				{
					return;
				}

				this->EnableWindowTitleBar = enable;
			}

			std::string WindowTitle = "GuardianWindow";
			Style WindowStyle = GE_STYLE_DEFAULTWINDOW;
			Theme WindowTheme = GE_THEME_DARK;

		private:
			bool EnableWindowTitleBar;

			friend class GWindow;
		};

		using Handle = HWND;

	public:
		GWindow();
		GWindow(const GWindow& other);
		GWindow(const Attribute& attribute);
		~GWindow();

		void InitializeWindow(const Attribute& attribute);

		void DisplayWindow();
		void UpdateWindow();
		void Render();

		void DestroyWindow();

		const bool& GetWindowRunning() const noexcept;

		const Handle& GetWindowHandle() const noexcept;
		const RECT& GetWindowArea() const noexcept;
		const Attribute& GetWindowAttribute() const noexcept;
		const std::string& GetWindowTitle() const noexcept;
		const Style& GetWindowStyle() const noexcept;
		const Theme& GetWindowTheme() const noexcept;

		static std::shared_ptr<GWindow> CreateNewWindow(const Attribute& attribute)
		{
			return std::make_shared<GWindow>(attribute);
		}

	private:
		static LRESULT WINAPI InitializeWindowMessageProcess(Handle handle, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI StaticWindowMessageProcess(Handle handle, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WINAPI ProcessWindowMessage(Handle handle, UINT message, WPARAM wParam, LPARAM lParam);

		void DrawDarkCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam);
		void DrawLightCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam);
		void DrawBlueCaption(bool isActive, Handle handle, UINT message, WPARAM wParam, LPARAM lParam);

		Handle WindowHandle;
		RECT WindowArea;
		Attribute WindowAttribute;
		MSG WindowMessage;
		bool IsTitleBarHovered = false;

		std::shared_ptr<GEventProcesser> WindowEventProcesser;
		bool IsWindowRunning;
	};
}

#endif
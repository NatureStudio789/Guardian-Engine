#ifndef _GE_GWIDGET_H_
#define _GE_GWIDGET_H_
#include <Profile/Core/GCore.h>
#include <Graphics/Math/Vector/GVector.h>

namespace GE
{
    namespace EUI
    {
        class GWidget
        {
        public:
            struct Style
            {
                enum Var
                {
                    GEStyleVar_Alpha,
                    GEStyleVar_DisabledAlpha,
                    GEStyleVar_WindowPadding,
                    GEStyleVar_WindowRounding,
                    GEStyleVar_WindowBorderSize,
                    GEStyleVar_WindowMinSize,
                    GEStyleVar_WindowTitleAlign,
                    GEStyleVar_ChildRounding,
                    GEStyleVar_ChildBorderSize,
                    GEStyleVar_PopupRounding,
                    GEStyleVar_PopupBorderSize,
                    GEStyleVar_FramePadding,
                    GEStyleVar_FrameRounding,
                    GEStyleVar_FrameBorderSize,
                    GEStyleVar_ItemSpacing,
                    GEStyleVar_ItemInnerSpacing,
                    GEStyleVar_IndentSpacing,
                    GEStyleVar_CellPadding,
                    GEStyleVar_ScrollbarSize,
                    GEStyleVar_ScrollbarRounding,
                    GEStyleVar_GrabMinSize,
                    GEStyleVar_GrabRounding,
                    GEStyleVar_TabRounding,
                    GEStyleVar_TabBorderSize,
                    GEStyleVar_TabBarBorderSize,
                    GEStyleVar_TabBarOverlineSize,
                    GEStyleVar_TableAngledHeadersAngle,
                    GEStyleVar_TableAngledHeadersTextAlign,
                    GEStyleVar_ButtonTextAlign,
                    GEStyleVar_SelectableTextAlign,
                    GEStyleVar_SeparatorTextBorderSize,
                    GEStyleVar_SeparatorTextAlign,
                    GEStyleVar_SeparatorTextPadding,
                    GEStyleVar_DockingSeparatorSize,
                    GEStyleVar_COUNT
                };

                Var Index;
                GVector2 Value;
            };

            struct Color
            {
                enum Category
                {
                    GEColorCategory_Text,
                    GEColorCategory_TextDisabled,
                    GEColorCategory_WindowBg,
                    GEColorCategory_ChildBg,
                    GEColorCategory_PopupBg,
                    GEColorCategory_Border,
                    GEColorCategory_BorderShadow,
                    GEColorCategory_FrameBg,
                    GEColorCategory_FrameBgHovered,
                    GEColorCategory_FrameBgActive,
                    GEColorCategory_TitleBg,
                    GEColorCategory_TitleBgActive,
                    GEColorCategory_TitleBgCollapsed,
                    GEColorCategory_MenuBarBg,
                    GEColorCategory_ScrollbarBg,
                    GEColorCategory_ScrollbarGrab,
                    GEColorCategory_ScrollbarGrabHovered,
                    GEColorCategory_ScrollbarGrabActive,
                    GEColorCategory_CheckMark,
                    GEColorCategory_SliderGrab,
                    GEColorCategory_SliderGrabActive,
                    GEColorCategory_Button,
                    GEColorCategory_ButtonHovered,
                    GEColorCategory_ButtonActive,
                    GEColorCategory_Header,
                    GEColorCategory_HeaderHovered,
                    GEColorCategory_HeaderActive,
                    GEColorCategory_Separator,
                    GEColorCategory_SeparatorHovered,
                    GEColorCategory_SeparatorActive,
                    GEColorCategory_ResizeGrip,
                    GEColorCategory_ResizeGripHovered,
                    GEColorCategory_ResizeGripActive,
                    GEColorCategory_TabHovered,
                    GEColorCategory_Tab,
                    GEColorCategory_TabSelected,
                    GEColorCategory_TabSelectedOverline,
                    GEColorCategory_TabDimmed,
                    GEColorCategory_TabDimmedSelected,
                    GEColorCategory_TabDimmedSelectedOverline,
                    GEColorCategory_DockingPreview,
                    GEColorCategory_DockingEmptyBg,
                    GEColorCategory_PlotLines,
                    GEColorCategory_PlotLinesHovered,
                    GEColorCategory_PlotHistogram,
                    GEColorCategory_PlotHistogramHovered,
                    GEColorCategory_TableHeaderBg,
                    GEColorCategory_TableBorderStrong,
                    GEColorCategory_TableBorderLight,
                    GEColorCategory_TableRowBg,
                    GEColorCategory_TableRowBgAlt,
                    GEColorCategory_TextLink,
                    GEColorCategory_TextSelectedBg,
                    GEColorCategory_DragDropTarget,
                    GEColorCategory_NavCursor,
                    GEColorCategory_NavWindowingHighlight,
                    GEColorCategory_NavWindowingDimBg,
                    GEColorCategory_ModalWindowDimBg,
                    GEColorCategory_COUNT,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                    GEColorCategory_TabActive = ImGuiCol_TabSelected,
                    GEColorCategory_TabUnfocused = ImGuiCol_TabDimmed,
                    GEColorCategory_TabUnfocusedActive = ImGuiCol_TabDimmedSelected,
                    GEColorCategory_NavHighlight = ImGuiCol_NavCursor,
#endif
                };

                Category Index;
                GVector4 Value;
            };

        public:
            void EnableWidgetRendering();
            void DisableWidgetRendering();

            const bool& IsEnableRendering() const;

            void AddStyleToWidget(Style style);
            void AddColorToWidget(Color color);
            void SetWidgetEventProcessFunction(const std::function<void()>& function);

            const std::vector<Style>& GetWidgetStyleList() const;
            const std::vector<Color>& GetWidgetColorList() const;

            virtual void Update() {}

            virtual void Render() {}

            const GUUID& GetWidgetId() const noexcept;

        protected:
            void CallWidgetEventProcessFunction();

            GUUID WidgetId = GUUID();

            bool EnableRendering = true;
            std::function<void()> WidgetEventProcessFunction;
            std::vector<Style> WidgetStyleList;
            std::vector<Color> WidgetColorList;
        };
    }
}

#endif
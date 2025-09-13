#ifndef _GE_GPANEL_H_
#define _GE_GPANEL_H_
#include "../Button/GButton.h"
#include "../Text/GText.h"
#include "../Button/GImageButton.h"
#include "../ColumnLayout/GColumnLayout.h"
#include "../TreeNode/GTreeNode.h"
#include "../Drag/GDrag.h"
#include "../InputBox/GInputBox.h"
#include "../Popup/GPopup.h"
#include "../MenuBar/GMenuBar.h"
#include "../MenuBar/Menu/GMenu.h"
#include "../MenuBar/MenuItem/GMenuItem.h"
#include "../Separator/GSeparator.h"
#include "../Manipulater/GManipulater.h"

namespace GE
{
    namespace EUI
    {
        class GPanel : public GWidget
        {
        public:
            enum Flag
            {
                GEPanelFlag_None = 0,
                GEPanelFlag_NoTitleBar = 1 << 0,
                GEPanelFlag_NoResize = 1 << 1,
                GEPanelFlag_NoMove = 1 << 2,
                GEPanelFlag_NoScrollbar = 1 << 3,
                GEPanelFlag_NoScrollWithMouse = 1 << 4,
                GEPanelFlag_NoCollapse = 1 << 5,
                GEPanelFlag_AlwaysAutoResize = 1 << 6,
                GEPanelFlag_NoBackground = 1 << 7,
                GEPanelFlag_NoSavedSettings = 1 << 8,
                GEPanelFlag_NoMouseInputs = 1 << 9,
                GEPanelFlag_MenuBar = 1 << 10,
                GEPanelFlag_HorizontalScrollbar = 1 << 11,
                GEPanelFlag_NoFocusOnAppearing = 1 << 12,
                GEPanelFlag_NoBringToFrontOnFocus = 1 << 13,
                GEPanelFlag_AlwaysVerticalScrollbar = 1 << 14,
                GEPanelFlag_AlwaysHorizontalScrollbar = 1 << 15,
                GEPanelFlag_NoNavInputs = 1 << 16,
                GEPanelFlag_NoNavFocus = 1 << 17,
                GEPanelFlag_UnsavedDocument = 1 << 18,
                GEPanelFlag_NoDocking = 1 << 19,
                GEPanelFlag_NoNav = GEPanelFlag_NoNavInputs | GEPanelFlag_NoNavFocus,
                GEPanelFlag_NoDecoration = GEPanelFlag_NoTitleBar | GEPanelFlag_NoResize | GEPanelFlag_NoScrollbar | GEPanelFlag_NoCollapse,
                GEPanelFlag_NoInputs = GEPanelFlag_NoMouseInputs | GEPanelFlag_NoNavInputs | GEPanelFlag_NoNavFocus,

                GEPanelFlag_ChildWindow = 1 << 24,
                GEPanelFlag_Tooltip = 1 << 25,
                GEPanelFlag_Popup = 1 << 26,
                GEPanelFlag_Modal = 1 << 27,
                GEPanelFlag_ChildMenu = 1 << 28,
                GEPanelFlag_DockNodeHost = 1 << 29,

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
                GEPanelFlag_AlwaysUseWindowPadding = 1 << 30,
                GEPanelFlag_NavFlattened = 1 << 31,
#endif
            };

        public:
            GPanel() = default;
            GPanel(const std::string& name);
            GPanel(const GPanel&) = default;
            virtual ~GPanel() = default;

            void SetPanelName(const std::string& name);

            void AddWidgetToPanel(std::shared_ptr<GWidget> widget);
            void ClearPanelWidgets();

            void AddPanelFlag(Flag flag);
            void AddPanelFlags(int flags);

            virtual void Update() override;

            virtual void Render() override;

            const std::string& GetPanelName() const noexcept;

        protected:
            void RenderPanelWidgets();
            void UpdatePanelWidgets();

            std::string PanelName;
            int PanelFlags;

            std::vector<std::shared_ptr<GWidget>> PanelWidgetList;
        };
    }
}

#endif
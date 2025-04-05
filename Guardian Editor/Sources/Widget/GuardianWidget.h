#ifndef _GE_GUARDIANWIDGET_H_
#define _GE_GUARDIANWIDGET_H_
#include <Profile/Core/GuardianCore.h>
#include <Graphics/Transform/Vector/GuardianVector.h>

namespace GE
{
	class GuardianWidget
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

	public:
		void EnableWidgetRendering();
		void DisableWidgetRendering();

		const bool& IsEnableRendering() const;

        void AddStyleToWidget(Style style);
        void SetWidgetEventProcessFunction(const std::function<void()>& function);

        const std::vector<Style>& GetWidgetStyleList() const;

		virtual void Update() {}

        /*Must call the CallWidgetEventProcessFunction in this function!!!*/
		virtual void Render() {}

	protected:
        /*This function must be called in every widget render function!*/
        void CallWidgetEventProcessFunction();

        bool EnableRendering = true;
        std::function<void()> WidgetEventProcessFunction;
        std::vector<Style> WidgetStyleList;
	};
}

#endif
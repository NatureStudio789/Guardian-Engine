#include "GuardianDockspace.h"

namespace GE
{
	namespace EUI
	{
		GuardianDockspace::GuardianDockspace(const std::string& name)
		{
			this->SetDockspaceName(name);
		}

		void GuardianDockspace::SetDockspaceName(const std::string& name)
		{
			this->DockspaceName = name;
		}

		void GuardianDockspace::AddWidgetToDockspace(std::shared_ptr<GuardianWidget> widget)
		{
			this->DockspaceWidgetList.push_back(widget);
		}

		void GuardianDockspace::ClearDockspaceWidgets()
		{
			this->DockspaceWidgetList.clear();
		}

		void GuardianDockspace::Update()
		{
			for (int i = 0; i < (int)this->DockspaceWidgetList.size(); i++)
			{
				this->DockspaceWidgetList[i]->Update();
			}
		}

		void GuardianDockspace::Render()
		{

		}

		const std::string& GuardianDockspace::GetDockspaceName() const noexcept
		{
			return this->DockspaceName;
		}
	}
}

#ifndef _GE_GUARDIANDOCKSPACE_H_
#define _GE_GUARDIANDOCKSPACE_H_
#include "../Panel/GuardianPanel.h"

namespace GE
{
	namespace EUI
	{
		class GuardianDockspace : public GuardianWidget
		{
		public:
			GuardianDockspace() = default;
			GuardianDockspace(const GString& name);
			GuardianDockspace(const GuardianDockspace& ) = default;
			virtual ~GuardianDockspace() = default;

			void SetDockspaceName(const GString& name);

			void AddWidgetToDockspace(std::shared_ptr<GuardianWidget> widget);
			void ClearDockspaceWidgets();

			void Update() override;

			void Render() override;

			const GString& GetDockspaceName() const noexcept;

		protected:
			GString DockspaceName;

			std::vector<std::shared_ptr<GuardianWidget>> DockspaceWidgetList;
		};
	}
}

#endif
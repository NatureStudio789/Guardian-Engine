#ifndef _GE_GDOCKSPACE_H_
#define _GE_GDOCKSPACE_H_
#include "../Panel/GPanel.h"

namespace GE
{
	namespace EUI
	{
		class GEditor;

		class GDockspace : public GWidget
		{
		public:
			GDockspace() = default;
			GDockspace(const std::string& name);
			GDockspace(const GDockspace& ) = default;
			virtual ~GDockspace() = default;

			void SetDockspaceName(const std::string& name);

			void AddEditorToDockspace(std::shared_ptr<GEditor> editor);
			void ClearDockspaceEditors();

			void Update() override;

			void Render() override;

			const std::string& GetDockspaceName() const noexcept;

		protected:
			std::string DockspaceName;

			std::vector<std::shared_ptr<GEditor>> DockspaceEditorList;
		};
	}
}

#endif
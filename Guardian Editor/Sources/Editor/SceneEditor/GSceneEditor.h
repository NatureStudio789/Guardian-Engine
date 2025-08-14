#ifndef _GE_GSCENEEDITOR_H_
#define _GE_GSCENEEDITOR_H_
#include <Editor/GEditor.h>
#include "../../EditorContext/GEditorContext.h"

namespace GE
{
	class GSceneEditor : public EUI::GEditor
	{
	public:
		void Initialize() override;
		
		void Update() override;

	private:
		void AddSceneNode(GEntity* entity, std::shared_ptr<EUI::GTreeNode> parent);

		std::shared_ptr<EUI::GPanel> SceneEditPanel;
		std::shared_ptr<EUI::GPanel> SceneRuntimePanel;
		std::shared_ptr<EUI::GImage> SceneEditImage;
		std::shared_ptr<EUI::GImage> SceneRuntimeImage;

		std::shared_ptr<EUI::GPanel> SceneHierarchyPanel;
		std::shared_ptr<EUI::GPanel> PropertiesPanel;
	};
}

#endif
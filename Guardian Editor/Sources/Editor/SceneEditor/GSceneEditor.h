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
		void UpdateEditPanel();

		void UpdateHierarchyPanel();
		void AddSceneNode(GEntity* entity, std::shared_ptr<EUI::GTreeNode> parent);
		
		void UpdatePropertiesPanel();
		void AddEntityProperties(std::shared_ptr<GEntity> entity);

		std::shared_ptr<EUI::GPanel> SceneEditPanel;
		std::shared_ptr<GRay> CastingRay;
		std::shared_ptr<EUI::GManipulater> EditManipulater;
		std::shared_ptr<EUI::GPanel> SceneRuntimePanel;
		std::shared_ptr<EUI::GImage> SceneEditImage;
		std::shared_ptr<EUI::GImage> SceneRuntimeImage;

		std::shared_ptr<EUI::GPanel> SceneHierarchyPanel;
		std::shared_ptr<EUI::GPanel> PropertiesPanel;
		GUUID SelectedEntityId;
	};
}

#endif
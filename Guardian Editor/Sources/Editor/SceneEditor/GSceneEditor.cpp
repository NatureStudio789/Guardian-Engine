#include "GSceneEditor.h"

namespace GE
{
	void GSceneEditor::Initialize()
	{
		EUI::GWidget::Style NoPaddingStyle;
		NoPaddingStyle.Index = NoPaddingStyle.GEStyleVar_WindowPadding;
		NoPaddingStyle.Value = GVector2(0.0f, 0.0f);
		
		{
			this->SceneEditPanel = std::make_shared<EUI::GPanel>("Scene");
			this->SceneEditPanel->AddStyleToWidget(NoPaddingStyle);

			this->SceneEditImage = std::make_shared<EUI::GImage>((EUI::GImage::Id)GRenderer::GetSceneRenderGraph()->GetEditFramebuffer()->
				GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle.ptr, GVector2());
			this->SceneEditPanel->AddWidgetToPanel(this->SceneEditImage);
			this->SceneEditPanel->SetWidgetEventProcessFunction([=]()
				{
					this->SceneEditImage->SetImageSize({ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
				});

			this->AddWidgetToEditor(this->SceneEditPanel);
		}

		{
			this->SceneRuntimePanel = std::make_shared<EUI::GPanel>("Game");
			this->SceneRuntimePanel->AddStyleToWidget(NoPaddingStyle);

			this->SceneRuntimeImage = std::make_shared<EUI::GImage>((EUI::GImage::Id)GRenderer::GetSceneRenderGraph()->GetRuntimeFramebuffer()->
				GetFramebufferRenderTarget()->GetTextureDescriptorHandle()->GPUHandle.ptr, GVector2());
			this->SceneRuntimePanel->AddWidgetToPanel(this->SceneRuntimeImage);
			this->SceneRuntimePanel->SetWidgetEventProcessFunction([=]()
				{
					this->SceneRuntimeImage->SetImageSize({ ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
				});

			this->AddWidgetToEditor(this->SceneRuntimePanel);
		}

		{
			this->SceneHierarchyPanel = std::make_shared<EUI::GPanel>("Hierarchy");

			this->AddWidgetToEditor(this->SceneHierarchyPanel);
		}

		{
			this->PropertiesPanel = std::make_shared<EUI::GPanel>("Properties");

			this->AddWidgetToEditor(this->PropertiesPanel);
		}
	}

	void GSceneEditor::Update()
	{
		this->SceneHierarchyPanel->ClearPanelWidgets();

		std::shared_ptr<EUI::GTreeNode> Root = std::make_shared<EUI::GTreeNode>(
			GSceneRegistry::GetActiveScene()->GetSceneName(), EUI::GTreeNode::GETreeNodeFlags_OpenOnArrow);
		this->SceneHierarchyPanel->AddWidgetToPanel(Root);

		for (auto& child : GSceneRegistry::GetActiveScene()->GetSceneRootEntity()->GetChildren())
		{
			this->AddSceneNode(child, Root);
		}
	}

	void GSceneEditor::AddSceneNode(GEntity* entity, std::shared_ptr<EUI::GTreeNode> parent)
	{
		std::shared_ptr<EUI::GTreeNode> node = std::make_shared<EUI::GTreeNode>(entity->GetEntityName(), EUI::GTreeNode::GETreeNodeFlags_OpenOnArrow);
		parent->AddWidgetToTreeNode(node);

		for (auto& child : entity->GetChildren())
		{
			this->AddSceneNode(child, node);
		}
	}
}

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
			this->SceneHierarchyPanel->SetWidgetEventProcessFunction([=]()
			{
				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				{
					this->SelectedEntityId = 0;
				}
			});

			this->AddWidgetToEditor(this->SceneHierarchyPanel);
		}

		{
			this->PropertiesPanel = std::make_shared<EUI::GPanel>("Properties");

			this->AddWidgetToEditor(this->PropertiesPanel);
		}
	}

	void GSceneEditor::Update()
	{
		this->UpdateHierarchyPanel();

		this->UpdatePropertiesPanel();
	}

	void GSceneEditor::UpdateHierarchyPanel()
	{
		this->SceneHierarchyPanel->ClearPanelWidgets();

		std::shared_ptr<EUI::GTreeNode> Root = std::make_shared<EUI::GTreeNode>(
			GSceneRegistry::GetActiveScene()->GetSceneName(), EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
		this->SceneHierarchyPanel->AddWidgetToPanel(Root);

		for (auto& child : GSceneRegistry::GetActiveScene()->GetSceneRootEntity()->GetChildren())
		{
			this->AddSceneNode(child, Root);
		}
	}

	void GSceneEditor::AddSceneNode(GEntity* entity, std::shared_ptr<EUI::GTreeNode> parent)
	{
		int NodeFlags = (this->SelectedEntityId == entity->GetEntityId()) ? EUI::GTreeNode::GETreeNodeFlags_Selected : EUI::GTreeNode::GETreeNodeFlags_None;
		std::shared_ptr<EUI::GTreeNode> node = std::make_shared<EUI::GTreeNode>(entity->GetEntityName(), NodeFlags);
		parent->AddWidgetToTreeNode(node);
		node->SetWidgetEventProcessFunction([=]()
		{
			if (ImGui::IsItemClicked())
			{
				this->SelectedEntityId = entity->GetEntityId();
			}
		});

		for (auto& child : entity->GetChildren())
		{
			this->AddSceneNode(child, node);
		}
	}

	void GSceneEditor::UpdatePropertiesPanel()
	{
		this->PropertiesPanel->ClearPanelWidgets();

		if (GSceneRegistry::GetActiveScene()->HasEntity(this->SelectedEntityId))
		{
			this->AddEntityProperties(GSceneRegistry::GetActiveScene()->GetEntity(this->SelectedEntityId));
		}
	}

	void GSceneEditor::AddEntityProperties(std::shared_ptr<GEntity> entity)
	{
		GUARDIAN_CHECK_POINTER(entity);

		if (entity->HasComponent<GTagComponent>())
		{
			this->PropertiesPanel->AddWidgetToPanel(std::make_shared<EUI::GTreeNode>("Tag", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen));
		}

		if (entity->HasComponent<GTransformComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Transform", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GTransformComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Position", &component.Transform.Position, 0.1f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Rotation", &component.Transform.Rotation, 0.1f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Scale", &component.Transform.Scale, 0.1f));
		}

		if (entity->HasComponent<GCameraComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Camera", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GCameraComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("FOV", &component.Camera->Projection.FOV, 0.1f, 30.0f, 100.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("NearZ", &component.Camera->Projection.NearZ, 0.001f, 0.01f, 1.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("FarZ", &component.Camera->Projection.FarZ, 0.1f, 1.0f));
		}

		if (entity->HasComponent<GPointLightComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Point Light", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);

			auto& component = entity->GetComponent<GPointLightComponent>();

			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Color", &component.PointLight->Color, 0.1f, 0.0f, 1.0f));
			ComponentNode->AddWidgetToTreeNode(std::make_shared<EUI::GDrag>("Strength", &component.PointLight->Strength, 0.1f, 1.0f));
		}

		if (entity->HasComponent<GModelComponent>())
		{
			auto& ComponentNode = std::make_shared<EUI::GTreeNode>("Model", EUI::GTreeNode::GETreeNodeFlags_DefaultOpen);
			this->PropertiesPanel->AddWidgetToPanel(ComponentNode);
		}
	}
}

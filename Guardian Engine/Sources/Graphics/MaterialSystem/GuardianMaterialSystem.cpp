#include "GuardianMaterialSystem.h"
#include "../Renderable/Mesh/GuardianMesh.h"
#include "../Renderer/GuardianRenderer.h"

namespace GE
{
    std::map<GString, std::shared_ptr<GuardianMaterial>> GuardianMaterialSystem::MaterialList;
    std::map<GString, std::shared_ptr<GuardianMesh>> GuardianMaterialSystem::MaterialSphereMeshList;


    const GuardianUUID& GuardianMaterialSystem::CreateNewMaterial(const GString& name)
    {
        if (MaterialList.count(name) > 0)
        {
            throw GUARDIAN_ERROR_EXCEPTION("The material called '" + name + "' has already been exists!");
        }

        MaterialList[name] = std::make_shared<GuardianMaterial>();
        GuardianRenderer::CreateRenderingRenderGraph(name + " Rendering", 800, 800);
        GuardianRenderer::SetRenderingRenderGraphCamera(name + " Rendering", 
            { {0.0f, 0.0f, -2.5f}, {0.0f, 0.0f, 0.0f}, {60.0f, 800.0f / 800.0f, 0.01f, 1000.0f} });

        GuardianRenderer::SetRenderingRenderGraphShaderGroup(name + " Rendering", GuardianShaderSystem::PBR_MAIN_SHADER);

        MaterialSphereMeshList[name] = std::make_shared<GuardianMesh>();
        MaterialSphereMeshList[name]->InitializeMesh(
            name + " Sphere", GuardianModelImporter("../Guardian Engine/Assets/Models/Sphere/Sphere.obj").GetModelMeshInstanceDataList());
        MaterialSphereMeshList[name]->MeshInstancesList[0]->SetMeshInstanceMaterial(MaterialList[name]->GetMaterialId());
        return MaterialList[name]->GetMaterialId();
    }

    void GuardianMaterialSystem::UpdateMaterialSystem()
    {
        for (auto& materialSphere : MaterialSphereMeshList)
        {
            materialSphere.second->UpdateMeshTransform(XMMatrixIdentity());

            GuardianLightProperties Properties;
            Properties.CameraPosition = { 0.0f, 0.0f, -2.5f };
            Properties.LightNumber = 1;
            Properties.PointLightList[0].LightColor = { 1.0f, 1.0f, 1.0f };
            Properties.PointLightList[0].LightPosition = { 0.5f, 1.0f, -3.0f };
            Properties.PointLightList[0].LightStrength = 200.0f;
            materialSphere.second->UpdateMeshLighting(Properties);

            materialSphere.second->SubmitToRenderer(materialSphere.first + " Rendering");
        }
    }

    std::shared_ptr<GuardianMaterial> GuardianMaterialSystem::GetMaterial(const GString& name)
    {
        if (MaterialList.count(name) <= 0)
        {
            throw GUARDIAN_ERROR_EXCEPTION("No materials called '" + name + "' are exists!");
        }

        return MaterialList[name];
    }

    std::shared_ptr<GuardianMaterial> GuardianMaterialSystem::GetMaterial(const GuardianUUID& id)
    {
        for (auto& iterator : MaterialList)
        {
            if (iterator.second->GetMaterialId() == id)
            {
                return iterator.second;
            }
        }

        throw GUARDIAN_ERROR_EXCEPTION("No materials with id : '" + std::to_string(id) + "' are exists!");
    }

    std::shared_ptr<GuardianFramebuffer> GuardianMaterialSystem::GetMaterialRenderingView(const GString& materialName)
    {
        return GuardianRenderer::GetRenderingRenderGraphFramebuffer(materialName + " Rendering");
    }

    std::shared_ptr<GuardianFramebuffer> GuardianMaterialSystem::GetMaterialRenderingView(const GuardianUUID& id)
    {
        return GuardianRenderer::GetRenderingRenderGraphFramebuffer(GetMaterialName(id) + " Rendering");
    }

    const GString& GuardianMaterialSystem::GetMaterialName(const GuardianUUID& id)
    {
        for (auto& iterator : MaterialList)
        {
            if (iterator.second->GetMaterialId() == id)
            {
                return iterator.first;
            }
        }

        throw GUARDIAN_ERROR_EXCEPTION("No materials with id : '" + std::to_string(id) + "' are exists!");
    }

    std::map<GString, std::shared_ptr<GuardianMaterial>> GuardianMaterialSystem::GetMaterialList()
    {
        return MaterialList;
    }
}

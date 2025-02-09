#include "GuardianPhysicsMaterial.h"

namespace guardian
{
	GuardianPhysicsMaterial::GuardianPhysicsMaterial()
	{
		this->MaterialObject = null;
		this->StaticFriction = 0.0f;
		this->DynamicFriction = 0.0f;
		this->Restitution = 0.0f;
	}

	GuardianPhysicsMaterial::GuardianPhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		this->InitializePhysicsMaterial(staticFriction, dynamicFriction, restitution);
	}

	GuardianPhysicsMaterial::GuardianPhysicsMaterial(const GuardianPhysicsMaterial& other)
	{
		this->MaterialObject = other.MaterialObject;
		this->StaticFriction = other.StaticFriction;
		this->DynamicFriction = other.DynamicFriction;
		this->Restitution = other.Restitution;
	}

	GuardianPhysicsMaterial::~GuardianPhysicsMaterial()
	{
	}

	void GuardianPhysicsMaterial::InitializePhysicsMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		this->StaticFriction = staticFriction;
		this->DynamicFriction = dynamicFriction;
		this->Restitution = restitution;

		this->MaterialObject = GuardianPhysicsEngine::CreatePhysicsMaterial(
			this->StaticFriction, this->DynamicFriction, this->Restitution);
		if (!this->MaterialObject)
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to create physics material!");
		}
	}

	void GuardianPhysicsMaterial::ResetStaticFriction(float staticFriction)
	{
		this->StaticFriction = staticFriction;
		this->MaterialObject->setStaticFriction(this->StaticFriction);
	}

	void GuardianPhysicsMaterial::ResetDynamicFriction(float dynamicFriction)
	{
		this->DynamicFriction = dynamicFriction;
		this->MaterialObject->setDynamicFriction(this->DynamicFriction);
	}

	void GuardianPhysicsMaterial::ResetRestitution(float restituion)
	{
		this->Restitution = restituion;
		this->MaterialObject->setRestitution(this->Restitution);
	}

	void GuardianPhysicsMaterial::Serialize(const GString& filePath)
	{
		YAML::Emitter MaterialOutput;
		MaterialOutput << YAML::BeginMap;
		MaterialOutput << YAML::Key << "Physics Material";

		MaterialOutput << YAML::BeginMap;
		MaterialOutput << YAML::Key << "Static Friction" << YAML::Value << this->StaticFriction;
		MaterialOutput << YAML::Key << "Dynamic Friction" << YAML::Value << this->DynamicFriction;
		MaterialOutput << YAML::Key << "Restitution" << YAML::Value << this->Restitution;
		MaterialOutput << YAML::EndMap;

		MaterialOutput << YAML::EndMap;

		std::ofstream MaterialFileOutput(filePath);
		MaterialFileOutput << MaterialOutput.c_str();
	}

	void GuardianPhysicsMaterial::Deserialize(const GString& filePath)
	{
		std::ifstream MaterialFile(filePath);
		std::stringstream MaterialFileStringStream;
		MaterialFileStringStream << MaterialFile.rdbuf();
		if (MaterialFileStringStream.str().empty())
		{
			throw GUARDIAN_ERROR_EXCEPTION("Failed to load material : '" + filePath + "' !");
		}

		YAML::Node MaterialData = YAML::Load(MaterialFileStringStream.str());
		auto MaterialParameters = MaterialData["Physics Material"];
		this->StaticFriction = MaterialParameters["Static Friction"].as<float>();
		this->DynamicFriction = MaterialParameters["Dynamic Friction"].as<float>();
		this->Restitution = MaterialParameters["Restitution"].as<float>();
	}

	PxMaterial* GuardianPhysicsMaterial::GetMaterialObject() noexcept
	{
		return this->MaterialObject;
	}

	const float GuardianPhysicsMaterial::GetStaticFriction() const noexcept
	{
		return this->StaticFriction;
	}

	const float GuardianPhysicsMaterial::GetDynamicFriction() const noexcept
	{
		return this->DynamicFriction;
	}

	const float GuardianPhysicsMaterial::GetRestitution() const noexcept
	{
		return this->Restitution;
	}
}
